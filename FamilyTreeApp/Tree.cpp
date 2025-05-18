#include "Tree.h"

#include <QSqlQuery>
#include <QSqlError>

Tree::Tree()
    : homePerson(nullptr)
{}

void Tree::addPerson(Person* person) {
    if (!person)
        return;

    QSqlQuery query;

    query.prepare("INSERT INTO Person (firstName, lastName, gender) "
                  "VALUES (:fname, :lname, :gender)");

    query.bindValue(":fname", QString::fromStdString(person->getFirstName()));
    query.bindValue(":lname", QString::fromStdString(person->getLastName()));
    query.bindValue(":gender", QString::fromStdString(GenderCast::toString(person->getGender())));

    if (query.exec()) {
        qDebug() << "Person added to database!";

        int id = query.lastInsertId().toInt();

        person->setID(id);

        if (people.empty())
            homePerson = person;

        people.insert({id, person});
    }

    else {
        qDebug() << "Failed to add person: " << query.lastError().text();
    }

}

void Tree::setHomePerson(Person* person) {
    for (const auto& pair : people) {
        if (pair.second == person) {
            homePerson = person;
            return;
        }
    }
    throw runtime_error("Cannot set home person as not found in tree.");
}

Person* Tree::getHomePerson() const {
    return homePerson;
}

Person* Tree::getPerson(const int& id) {
    if (people.count(id))
        return people[id];
    return nullptr;
}

void Tree::deletePerson(const int& id) {
    // if (!people.count(id))
    //     return;

    // Person* person = people[id];

    // if (person == homePerson)
    //     homePerson = nullptr;

    // people.erase(id);

    // for (const auto& familyPair : families) {
    //     Family* family = familyPair.second;
    //     if (person == family->getFather())
    //         family->setFather(nullptr);
    //     if (person == family->getMother())
    //         family->setMother(nullptr);
    //     // TODO
    //     // Handle child case too
    //     // Need to remove from vector
    // }

    // delete person;

    QSqlQuery query;

    query.prepare("DELETE FROM Person WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec())
        qDebug() << "Person " + to_string(id) + " is removed from database!";
    else
        qDebug() << "Failed to remove person: " << query.lastError().text();

    clear();
    load();
}

// Family* Tree::createFamily(Person* father, Person* mother) {
//     Family* family = new Family(father, mother);
//     if (father)
//         father->addToMarriedFamily(family);
//     if (mother)
//         mother->addToMarriedFamily(family);
//     families.push_back(family);
//     return family;
// }


map<int, Person*> Tree::getPeople() const {
    return people;
}
map<int, Family*> Tree::getFamilies() const {
    return families;
}

void Tree::addFamily(Family* family) {
    if (!family)
        return;

    QSqlQuery query;

    query.prepare("INSERT INTO Family (fatherID, motherID) VALUES (:fatherID, :motherID)");

    Person* father = family->getFather();
    Person* mother = family->getMother();

    query.bindValue(":fatherID", father ? QVariant(father->getID()) : QVariant());
    query.bindValue(":motherID", mother ? QVariant(mother->getID()) : QVariant());

    if (query.exec()) {
        int id = query.lastInsertId().toInt();

        family->setID(id);

        for (const auto& child : family->getChildRelations()) {
            query.prepare("INSERT INTO FamilyChild (childID, FamilyID) VALUES (:childID, :familyID)");
            query.bindValue(":childID", child.person->getID());
            query.bindValue(":familyID", id);
            query.exec();
        }

        qDebug() << "Family successfully inserted!";

        families.insert({id, family});
    }
    else
        qDebug() << "Failed to insert family: " << query.lastError().text();
}

void Tree::deleteFamily(const int& id) {
    if (!families.count(id))
        return;

    families.erase(id);

    QSqlQuery query;

    query.prepare("DELETE FROM Family WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec())
        qDebug() << "Family " + to_string(id) + " is removed from database!";
    else
        qDebug() << "Failed to remove family: " << query.lastError().text();
}

// DATABASE

void Tree::setupDatabase(const string& path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(path));

    QSqlQuery query;

    if (!db.open()) {
        qDebug() << "Database error: " << db.lastError().text();
    } else {
        query.exec("PRAGMA foreign_keys = ON");
        qDebug() << "Database opened and foreign keys enabled!";
    }

    if (query.exec(
            "CREATE TABLE IF NOT EXISTS Person ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT"
            ", firstName TEXT"
            ", lastName TEXT"
            ", gender TEXT CHECK(gender IN ('Male', 'Female', 'Unknown'))"
            ")"
            ))
        qDebug() << "Table Person created if it does not exist!";
    else
        qDebug() << "Table Person creation error: " << query.lastError().text();

    if (query.exec(
            "CREATE TABLE IF NOT EXISTS Family ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT"
            ", fatherID INTEGER"
            ", motherID INTEGER"
            ", FOREIGN KEY(fatherID) REFERENCES Person(id) ON DELETE SET NULL"
            ", FOREIGN KEY(motherID) REFERENCES Person(id) ON DELETE SET NULL"
            ")"
            ))
        qDebug() << "Table Family created if it does not exist!";
    else
        qDebug() << "Table Family creation error: " << query.lastError().text();

    if (query.exec(
            "CREATE TABLE IF NOT EXISTS FamilyChild ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT"
            ", childID INTEGER"
            ", familyID INTEGER"
            ", FOREIGN KEY(childID) REFERENCES Person(id) ON DELETE CASCADE"
            ", FOREIGN KEY(familyID) REFERENCES Family(id) ON DELETE CASCADE"
            ")"
            ))
        qDebug() << "Table FamilyChild created if it does not exist!";
    else
        qDebug() << "Table FamilyChild creation error: " << query.lastError().text();
}

void Tree::load() {
    QSqlQuery query;
    if (!query.exec("SELECT id, firstName, lastName, gender FROM Person")) {
        qDebug() << "Select Person failed: " << query.lastError().text();
    }
    while (query.next()) {
        Person* person = new Person;
        int id = query.value(0).toInt();
        person->setID(id);
        person->setFirstName(query.value(1).toString().toStdString());
        person->setLastName(query.value(2).toString().toStdString());
        person->setGender(GenderCast::fromString(query.value(3).toString().toStdString()));
        people[id] = person;
    }

    if (!query.exec("SELECT id, fatherID, motherID FROM Family")) {
        qDebug() << "Select Family failed: " << query.lastError().text();
    }

    while (query.next()) {
        Family* family = new Family;
        int id = query.value(0).toInt();
        family->setID(id);
        family->setFather(getPerson(query.value(1).toInt()));
        family->setMother(getPerson(query.value(2).toInt()));
        families[id] = family;
    }
}

void Tree::updatePersonInDatabase(const int& id) {
    if (!people.count(id))
        return;

    Person* person = people[id];

    QSqlQuery query;
    query.prepare("UPDATE Person SET firstName = :firstName, lastName = :lastName, gender = :gender WHERE id = :id");

    query.bindValue(":firstName", QString::fromStdString(person->getFirstName()));
    query.bindValue(":lastName", QString::fromStdString(person->getLastName()));
    query.bindValue(":gender", QString::fromStdString(GenderCast::toString(person->getGender())));
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError().text();
    } else {
        qDebug() << "Updated Person " << id;
    }

}

void Tree::clear() {
    homePerson = nullptr;
    people.clear();
    families.clear();
}
