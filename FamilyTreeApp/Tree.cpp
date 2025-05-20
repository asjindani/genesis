#include "Tree.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Tree::Tree()
    : id(1)
    , name("")
    , homePerson(nullptr)
{}

int Tree::getID() const {
    return id;
}

void Tree::setID(const int& id) {
    this->id = id;
}

bool Tree::insertPerson(Person* person) {
    if (!person)
        return false;

    QSqlQuery query;

    query.prepare("INSERT INTO Person (id, treeID, firstName, lastName, gender, birthDate) "
                  "VALUES (:id, :treeid, :fname, :lname, :gender, :bdate)");

    if (person->getID() > 0)
        query.bindValue(":id", person->getID());

    query.bindValue(":treeid", id);

    query.bindValue(":fname", QString::fromStdString(person->getFirstName()));
    query.bindValue(":lname", QString::fromStdString(person->getLastName()));
    query.bindValue(":gender", QString::fromStdString(GenderCast::toString(person->getGender())));
    query.bindValue(":bdate", person->getBirthDate());

    if (!query.exec()) {
        qDebug() << "Failed to insert person: " << query.lastError().text();
        return false;
    }

    qDebug() << "Person added to database!";
    int id = query.lastInsertId().toInt();
    person->setID(id);
    return true;
}

void Tree::addPerson(Person* person) {
    if (!person)
        return;

    if (insertPerson(person)) {
        // if (people.empty())
        //     homePerson = person;
        people.insert({person->getID(), person});
    }
}

void Tree::setHomePerson(Person* person) {
    if (!person || personExists(person->getID()))
        homePerson = person;
    else
        homePerson = nullptr;
}

Person* Tree::getHomePerson() const {
    return homePerson;
}

Person* Tree::getPerson(const int& id) {
    if (personExists(id))
        return people[id];
    return nullptr;
}

Family* Tree::getFamily(const int& id) {
    if (familyExists(id))
        return families[id];
    return nullptr;
}

void Tree::deletePerson(const int& id) {
    if (!personExists(id))
        return;

    QSqlQuery query;

    query.prepare("DELETE FROM Person WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec())
        qDebug() << "Person " + to_string(id) + " is removed from database!";
    else
        qDebug() << "Failed to remove person: " << query.lastError().text();

    delete people[id];
    people.erase(id);
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

bool Tree::insertFamily(Family* family) {
    if (!family)
        return false;

    QSqlQuery query;

    query.prepare("INSERT INTO Family (id, treeID, fatherID, motherID) VALUES (:id, :treeid, :fatherID, :motherID)");

    if (family->getID() > 0)
        query.bindValue(":id", family->getID());

    query.bindValue(":treeid", id);

    if (const auto& father = family->getFather())
        query.bindValue(":fatherID", father->getID());

    if (const auto& mother = family->getMother())
        query.bindValue(":motherID", mother->getID());

    if (!query.exec()) {
        qDebug() << "Failed to insert family: " << query.lastError().text();
        return false;
    }

    int id = query.lastInsertId().toInt();
    family->setID(id);

    for (const auto& child : family->getChildren()) {
        query.prepare("INSERT INTO FamilyChild (childID, FamilyID) VALUES (:childID, :familyID)");
        query.bindValue(":childID", child->getID());
        query.bindValue(":familyID", id);
        query.exec();
    }

    qDebug() << "Family successfully inserted!";
    return true;
}

void Tree::addFamily(Family* family) {
    if (!family)
        return;

    if (insertFamily(family))
        families.insert({family->getID(), family});
}

void Tree::deleteFamily(const int& id) {
    if (!familyExists(id))
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

void Tree::connect(const string& path) {

    // Connecting to the database

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(path));

    if (!db.open()) {
        qDebug() << "Database error: " << db.lastError().text();
        return;
    }

    // Setting pragmas

    QSqlQuery query;

    if (query.exec("PRAGMA foreign_keys = ON"))
        qDebug() << "Database opened and foreign keys enabled!";

    // Loading schema file

    QFile file(":/schema.sql");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open SQL file: " << path;
        return;
    }

    QTextStream in(&file);
    QString script = in.readAll();
    file.close();

    // Executing queries

    QStringList queries = script.split(';', Qt::SkipEmptyParts);

    for (auto& queryText : queries) {
        queryText = queryText.trimmed();
        if (queryText.isEmpty())
            continue;
        if (query.exec(queryText))
            qDebug() << "Table is ready!";
        else
            qDebug() << "Query failed: " << query.lastError().text();
    }

    // if (query.exec(
    //         "CREATE TABLE IF NOT EXISTS Person ("
    //         "id INTEGER PRIMARY KEY AUTOINCREMENT"
    //         ", firstName TEXT"
    //         ", lastName TEXT"
    //         ", gender TEXT CHECK(gender IN ('Male', 'Female', 'Unknown'))"
    //         ", birthDate DATE"
    //         ")"
    //         ))
    //     qDebug() << "Table Person is ready!";
    // else
    //     qDebug() << "Table Person error: " << query.lastError().text();

    // if (query.exec(
    //         "CREATE TABLE IF NOT EXISTS Family ("
    //         "id INTEGER PRIMARY KEY AUTOINCREMENT"
    //         ", fatherID INTEGER"
    //         ", motherID INTEGER"
    //         ", FOREIGN KEY(fatherID) REFERENCES Person(id) ON DELETE SET NULL"
    //         ", FOREIGN KEY(motherID) REFERENCES Person(id) ON DELETE SET NULL"
    //         ")"
    //         ))
    //     qDebug() << "Table Family is ready!";
    // else
    //     qDebug() << "Table Family error: " << query.lastError().text();

    // if (query.exec(
    //         "CREATE TABLE IF NOT EXISTS FamilyChild ("
    //         "id INTEGER PRIMARY KEY AUTOINCREMENT"
    //         ", childID INTEGER"
    //         ", familyID INTEGER"
    //         ", FOREIGN KEY(childID) REFERENCES Person(id) ON DELETE CASCADE"
    //         ", FOREIGN KEY(familyID) REFERENCES Family(id) ON DELETE CASCADE"
    //         ")"
    //         ))
    //     qDebug() << "Table FamilyChild is ready!";
    // else
    //     qDebug() << "Table FamilyChild error: " << query.lastError().text();
}

void Tree::load() {
    if (!db.isOpen())
        return;

    QSqlQuery query;

    if (!query.exec("SELECT id, firstName, lastName, gender, birthDate FROM Person")) {
        qDebug() << "Select Person failed: " << query.lastError().text();
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        Person* person = (personExists(id) ? people[id] : new Person);
        person->setID(id);
        person->setFirstName(query.value(1).toString().toStdString());
        person->setLastName(query.value(2).toString().toStdString());
        person->setGender(query.value(3).toString().toStdString());
        person->setBirthDate(query.value(4).toDate());
        people[id] = person;
    }

    if (!query.exec("SELECT id, fatherID, motherID FROM Family")) {
        qDebug() << "Select Family failed: " << query.lastError().text();
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        Family* family = (familyExists(id) ? families[id] : new Family);
        family->setID(id);
        family->setFather(getPerson(query.value(1).toInt()));
        family->setMother(getPerson(query.value(2).toInt()));
        families[id] = family;
    }

    if (!query.exec("SELECT childID, familyID FROM FamilyChild")) {
        qDebug() << "Select FamilyChild failed: " << query.lastError().text();
    }

    while (query.next()) {
        int childID = query.value(0).toInt();
        int familyID = query.value(1).toInt();
        if (Family* family = getFamily(familyID))
            if (!family->hasChild(childID))
                if (Person* child = getPerson(childID))
                    family->addChild(child);
    }
}

void Tree::updatePerson(const int& id) {
    if (!personExists(id))
        return;

    Person* person = people[id];

    QSqlQuery query;
    query.prepare("UPDATE Person SET firstName = :firstName, lastName = :lastName, gender = :gender, birthDate = :bdate WHERE id = :id");

    query.bindValue(":firstName", QString::fromStdString(person->getFirstName()));
    query.bindValue(":lastName", QString::fromStdString(person->getLastName()));
    query.bindValue(":gender", QString::fromStdString(GenderCast::toString(person->getGender())));
    query.bindValue(":bdate", person->getBirthDate());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError().text();
    } else {
        qDebug() << "Updated Person " << id;
    }

    query.prepare("UPDATE Family SET fatherID = NULL WHERE fatherID = :id AND :id IN (SELECT id FROM Person WHERE id = :id AND Gender = 'Female')");
    query.bindValue(":id", id);

    if (!query.exec())
        qDebug() << "Query failed: " << query.lastError().text();


    query.prepare("UPDATE Family SET motherID = NULL WHERE motherID = :id AND :id IN (SELECT id FROM Person WHERE id = :id AND Gender = 'Male')");
    query.bindValue(":id", id);

    if (!query.exec())
        qDebug() << "Query failed: " << query.lastError().text();

    load();
}

void Tree::updateFamily(const int& id) {
    if (!familyExists(id))
        return;

    Family* family = families[id];

    QSqlQuery query;
    query.prepare("UPDATE Family SET fatherID = :fatherID, motherID = :motherID WHERE id = :id");

    if (const auto& father = family->getFather())
        query.bindValue(":fatherID", father->getID());
    if (const auto& mother = family->getMother())
        query.bindValue(":motherID", mother->getID());

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError().text();
    } else {
        QSet<int> oldChildIDs, newChildIDs;

        for (const auto& child : family->getChildRelations())
            newChildIDs.insert(child.person->getID());

        query.prepare("SELECT childID FROM FamilyChild WHERE familyID = :familyID");
        query.bindValue(":familyID", id);

        if (query.exec())
            while (query.next())
                oldChildIDs.insert(query.value(0).toInt());

        // Insert new children into table
        for (int childID : newChildIDs - oldChildIDs) {
            query.prepare("INSERT INTO FamilyChild (familyID, childID) VALUES (:familyID, :childID)");
            query.bindValue(":familyID", id);
            query.bindValue(":childID", childID);
            query.exec();
        }

        // Delete removed children from table
        for (int childID : oldChildIDs - newChildIDs) {
            query.prepare("DELETE FROM FamilyChild WHERE familyID = :familyID AND childID = :childID");
            query.bindValue(":familyID", id);
            query.bindValue(":childID", childID);
            query.exec();
        }

        qDebug() << "Updated Family " << id;
    }
}

set<Person*> Tree::getAncestors(const int& id) {
    if (!personExists(id))
        return {};

    set<Person*> ancestors;

    for (const auto& family : getBirthFamilies(id)) {
        if (const auto& father = family->getFather()) {
            ancestors.insert(father);
            set<Person*> temp = getAncestors(father->getID());
            ancestors.insert(temp.begin(), temp.end());
        }
        if (const auto& mother = family->getMother()) {
            ancestors.insert(mother);
            set<Person*> temp = getAncestors(mother->getID());
            ancestors.insert(temp.begin(), temp.end());
        }
    }

    return ancestors;
}

vector<Person*> Tree::getChildren(const int& id) {
    if (!personExists(id))
        return {};

    vector<Person*> children;

    QSqlQuery query;

    for (const auto& family : getMarriedFamilies(id)) {

        query.prepare("SELECT childID FROM FamilyChild WHERE familyID = :familyID");
        query.bindValue(":familyID", family->getID());

        if (query.exec())
            while (query.next()) {
                int childID = query.value(0).toInt();
                if (Person* child = getPerson(childID))
                    children.push_back(child);
            }
    }

    return children;
}

set<Person*> Tree::getDescendants(const int& id) {
    if (!personExists(id))
        return {};

    set<Person*> descendants;

    for (const auto& child : getChildren(id)) {
        descendants.insert(child);
        set<Person*> temp = getDescendants(child->getID());
        descendants.insert(temp.begin(), temp.end());
    }

    return descendants;
}

set<Family*> Tree::getBirthFamilies(const int& personID) {
    if (!personExists(personID))
        return {};

    QSqlQuery query;
    set<Family*> birthFamilies;

    query.prepare("SELECT familyID FROM FamilyChild WHERE childID = :childID");
    query.bindValue(":childID", personID);

    if (query.exec())
        while (query.next()) {
            int familyID = query.value(0).toInt();
            if (Family* family = getFamily(familyID))
                birthFamilies.insert(family);
        }

    return birthFamilies;
}

Family* Tree::getBirthFamily(const int& personID) {
    set<Family*> birthFamilies = getBirthFamilies(personID);
    if (!birthFamilies.empty())
        return *birthFamilies.begin();
    return nullptr;
}

set<Family*> Tree::getMarriedFamilies(const int& personID) {
    if (!personExists(personID))
        return {};

    set<Family*> marriedFamilies;
    QSqlQuery query;

    query.prepare("SELECT id FROM Family WHERE fatherID = :personID OR motherID = :personID");
    query.bindValue(":personID", personID);

    if (query.exec())
        while (query.next()) {
            int id = query.value(0).toInt();
            if (Family* family = getFamily(id))
                marriedFamilies.insert(family);
        }

    return marriedFamilies;
}

bool Tree::personExists(const int& id) const {
    return people.count(id);
}

bool Tree::familyExists(const int& id) const {
    return families.count(id);
}

void Tree::saveFile(const string& path) {
    QJsonArray peopleArray;

    for (const auto& [id, person] : people) {
        QJsonObject obj;
        obj["id"] = id;
        obj["firstName"] = QString::fromStdString(person->getFirstName());
        obj["lastName"] = QString::fromStdString(person->getLastName());
        obj["gender"] = QString::fromStdString(GenderCast::toString(person->getGender()));
        obj["birthDate"] = person->getBirthDate().toString(Qt::ISODate);
        peopleArray.append(obj);
    }

    QJsonArray familiesArray;
    for (const auto& [id, family] : families) {
        QJsonObject obj;
        obj["id"] = id;
        obj["fatherID"] = family->getFather() ? family->getFather()->getID() : 0;
        obj["motherID"] = family->getMother() ? family->getMother()->getID() : 0;

        QJsonArray childIDs;
        for (auto child : family->getChildren()) {
            childIDs.append(child->getID());
        }
        obj["children"] = childIDs;
        familiesArray.append(obj);
    }

    QJsonObject root;
    root["people"] = peopleArray;
    root["families"] = familiesArray;

    QJsonDocument doc(root);
    QFile file(QString::fromStdString(path));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void Tree::loadFile(const string& path) {
    QFile file(QString::fromStdString(path));

    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    file.close();

    if (!doc.isObject())
        return;

    QJsonObject root = doc.object();
    QJsonArray peopleArray = root["people"].toArray();
    QJsonArray familiesArray = root["families"].toArray();

    for (auto v : peopleArray) {
        QJsonObject obj = v.toObject();
        int id = obj["id"].toInt();
        Person* person = (personExists(id) ? people[id] : new Person);
        person->setID(id);
        person->setFirstName(obj["firstName"].toString().toStdString());
        person->setLastName(obj["lastName"].toString().toStdString());
        person->setGender(obj["gender"].toString().toStdString());
        person->setBirthDate(QDate::fromString(obj["birthDate"].toString(), Qt::ISODate));
        people[id] = person;
    }

    for (auto v : familiesArray) {
        QJsonObject obj = v.toObject();
        int id = obj["id"].toInt();

        Family* family = (familyExists(id) ? families[id] : new Family);

        family->setID(id);
        int fatherID = obj["fatherID"].toInt();
        int motherID = obj["motherID"].toInt();

        if (personExists(fatherID))
            family->setFather(getPerson(fatherID));
        if (personExists(motherID))
            family->setMother(getPerson(motherID));

        for (auto c : obj["children"].toArray()) {
            int childID = c.toInt();
            if (personExists(childID))
                family->addChild(getPerson(childID));
        }

        families[id] = family;
    }

}

void Tree::clear() {
    for (auto [id, person] : people)
        delete person;
    people.clear();
    homePerson = nullptr;
    for (auto [id, family] : families)
        delete family;
    families.clear();
}

void Tree::autosave() {
    QSqlQuery query;

    query.prepare("DELETE FROM Tree WHERE id = :id");
    query.bindValue(":id", 1);
    query.exec();

    // query.prepare("DELETE FROM Tree WHERE id = :id");
    // query.bindValue(":id", id);
    // query.exec();

    query.prepare("INSERT INTO Tree (id, name, homePersonID) VALUES (:id, :name, :homePersonID)");
    query.bindValue(":id", id);
    query.bindValue(":name", QString::fromStdString(name));
    if (homePerson)
        query.bindValue(":homePersonID", homePerson->getID());

    if (query.exec())
        qDebug() << "New tree inserted";
    else
        qDebug() << "Failed to insert tree: " << query.lastError().text();

    for (const auto& [id, person] : people)
        insertPerson(person);
    for (const auto& [id, family] : families)
        insertFamily(family);
}

bool Tree::empty() const {
    return people.empty() && families.empty();
}

Tree::~Tree() {
    clear();
}
