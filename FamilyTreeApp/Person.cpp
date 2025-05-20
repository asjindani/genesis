#include <set>
#include <algorithm>

#include "Person.h"
#include "Enum.h"

#include <QSqlDatabase>
#include <QSqlQuery>

// CONSTRUCTOR

Person::Person()
    : id(0)
    , firstName("")
    , lastName("")
    , gender(Gender::Unknown)
{}

// ATTRIBUTE GETTERS

int Person::getID() const {
    return id;
}

string Person::getFirstName() const {
    return firstName;
}

string Person::getLastName() const {
    return lastName;
}

Gender Person::getGender() const {
    return gender;
}

QDate Person::getBirthDate() const {
    return birthDate;
}

// SETTERS

void Person::setID(const int& id) {
    this->id = id;
}

void Person::setFirstName(const string& name) {
    firstName = name;
}

void Person::setLastName(const string& name) {
    lastName = name;
}

void Person::setGender(const Gender& gender) {
    this->gender = gender;
}

void Person::setGender(const string& gender) {
    setGender(GenderCast::fromString(gender));
}

void Person::setBirthDate(const QDate& date) {
    birthDate = date;
}

// OTHER GETTERS

string Person::getName() const {
    if (lastName.empty())
        return firstName;
    return firstName + ' ' + lastName;
}

string Person::getRepr() const {
    return getName();
    // string name = getName();
    // if (name.empty())
    //     return '[' + to_string(id) + ']';
    // return "[" + to_string(id) + "] " + name;
}

// void Person::setParents(Person* father, Person* mother, const Relation& type) {
//     if (father->getGender() != Male)
//         throw runtime_error("Father must be male");

//     if (mother->getGender() != Female)
//         throw runtime_error("Mother must be female");

//     if (type == Biological)
//         biologicalParents = {father, mother};
//     else
//         throw runtime_error("Not implemented");
// }


// pair<Person*, Person*> Person::getParents(const Relation& relation) const {
//     if (relation == Biological)
//         return biologicalParents;
//     else
//         throw runtime_error("Not implemented");
// }

// void Person::addToBirthFamily(Family* family) {
//     birthFamilies.push_back(family);
// }

// void Person::addToMarriedFamily(Family* family) {
//     marriedFamilies.push_back(family);
// }

// void Person::removeBirthFamily(Family* family) {
//     // for (auto it = birthFamilies.begin(); it != birthFamilies.end(); it++) {
//     //     if ()
//     // }

//     auto it = std::find(birthFamilies.begin(), birthFamilies.end(), family);

//     if (it != birthFamilies.end())
//         birthFamilies.erase(it);
// }

// void Person::removeMarriedFamily(Family* family) {
//     auto it = find(marriedFamilies.begin(), marriedFamilies.end(), family);

//     if (it != marriedFamilies.end())
//         marriedFamilies.erase(it);
// }




// vector<pair<ParentRelation, ParentRelation>> Person::getParentRelations() const {
//     vector<pair<ParentRelation, ParentRelation>> v;
//     for (const auto& family : childFamilies) {
//         ChildRelation r = family->getChildRelation(id);
//         v.push_back({{family->getFather(), r.fatherRelation}, {family->getMother(), r.motherRelation}});
//     }
//     return v;
// }

// vector<vector<SiblingRelation>> Person::getSiblingRelations() const {
//     vector<vector<SiblingRelation>> s;
//     for (const auto& family : childFamilies) {
//         ChildRelation childRelation = family->getChildRelation(id);
//         vector<SiblingRelation> v;
//         for (const auto siblingRelation : family->getChildRelations()) {
//             if (siblingRelation.person->getID() == childRelation.person->getID())
//                 continue;
//             SiblingType type = SiblingType::Step;
//             if (childRelation.fatherRelation == Relation::Biological && siblingRelation.fatherRelation == Relation::Biological) {
//                 type = SiblingType::Half;
//             }
//             if (childRelation.motherRelation == Relation::Biological && siblingRelation.motherRelation == Relation::Biological) {
//                 if (type == SiblingType::Half)
//                     type = SiblingType::Full;
//                 else
//                     type = SiblingType::Half;
//             }
//             v.push_back({siblingRelation.person, type});
//         }
//         s.push_back(v);
//     }
//     return s;
// }

// vector<FamilyRecord> Person::getBirthFamilyRecords() const {
//     vector<FamilyRecord> records;

//     for (const auto& family : birthFamilies) {
//         FamilyRecord record;

//         record.father = family->getFather();
//         record.mother = family->getMother();

//         auto self = family->getChildRelation(id);
//         record.fatherRelation = self.fatherRelation;
//         record.motherRelation = self.motherRelation;

//         for (const auto& sibling : family->getChildRelations()) {

//             // Excluding self from siblings
//             if (sibling.person->getID() == id)
//                 continue;

//             SiblingType type = SiblingType::Step;

//             if (record.father && self.fatherRelation == Relation::Biological && sibling.fatherRelation == Relation::Biological)
//                 type = SiblingType::Half;

//             if (record.mother && self.motherRelation == Relation::Biological && sibling.motherRelation == Relation::Biological)
//                 if (type == SiblingType::Half)
//                     type = SiblingType::Full;
//                 else
//                     type = SiblingType::Half;

//             record.siblings.push_back(sibling.person);
//             record.siblingRelations.push_back(type);
//         }
//         records.push_back(record);
//     }
//     return records;
// }

// vector<Family*> Person::getMarriedFamilyRecords() const {
//     return marriedFamilies;
// }


// OUTPUT

ostream& operator<<(ostream& out, const Relation& relation) {
    switch (relation) {
    case Relation::Biological:
        out << "Biological";
        break;
    case Relation::Foster:
        out << "Foster";
        break;
    case Relation::Adoptive:
        out << "Adoptive";
        break;
    }
    return out;
}

ostream& operator<<(ostream& out, const SiblingType& s) {
    switch (s) {
    case SiblingType::Full:
        out << "Full";
        break;
    case SiblingType::Half:
        out << "Half";
        break;
    case SiblingType::Step:
        out << "Step";
        break;
    }
    return out;
}

ostream& operator<<(ostream& out, const Gender& gender) {
    out << GenderCast::toString(gender);
    return out;
}

ostream& operator<<(ostream& out, const Person& person) {
    out << "PERSON (ID " << person.getID() << ")" << endl;
    out << "\tName: " << person.getName() << endl;
    out << endl;

    // for (const auto& record : person.getBirthFamilyRecords()) {
    //     out << "\tParents: ";
    //     if (record.father) out << record.father->getName() << " (" << record.fatherRelation << " Father) ";
    //     if (record.mother) out << record.mother->getName() << " (" << record.motherRelation << " Mother)";
    //     out << endl;

    //     out << "\tSiblings: ";
    //     for (int i = 0; i < record.siblings.size(); i++) {
    //         Person* sibling = record.siblings[i];
    //         SiblingType type = record.siblingRelations[i];

    //         out << sibling->getName() << " (" << type << " ";
    //         switch (sibling->gender) {
    //         case Gender::Male:
    //             out << "Brother";
    //             break;
    //         case Gender::Female:
    //             out << "Sister";
    //             break;
    //         case Gender::Unknown:
    //             out << "Sibling";
    //             break;
    //         }
    //         out << ") " << endl;
    //     }
    //     out << endl;
    // }

    // for (const auto& family : person.getMarriedFamilyRecords()) {
    //     out << "\tPartner: ";
    //     if (family->getFather()->getID() != person.getID())
    //         out << family->getFather()->getName() << endl;
    //     else
    //         out << family->getMother()->getName() << endl;
    //     out << "\tChildren: ";
    //     for (const auto& child : family->getChildRelations()) {
    //         out << child.person->getName() << " (";
    //         switch (child.person->getGender()) {
    //         case Gender::Male:
    //             out << "Son";
    //             break;
    //         case Gender::Female:
    //             out << "Daughter";
    //             break;
    //         case Gender::Unknown:
    //             out << "Child";
    //             break;
    //         }
    //         out << ") ";
    //     }
    //     out << endl;
    // }

    out << endl;



    // for (const auto& parents : person.getParentRelations()) {
    //     ParentRelation father = parents.first;
    //     ParentRelation mother = parents.second;
    //     if (!father.first && !mother.first)
    //         continue;
    //     out << "\tParents: ";
    //     if (father.first) out << father.first->getName() << " (" << father.second << " Father) ";
    //     if (mother.first) out << mother.first->getName() << " (" << mother.second << " Mother)";
    //     out << endl;
    // }

    // for (const auto& siblings : person.getSiblingRelations()) {
    //     out << "\tSiblings: ";
    //     for (const auto& s : siblings) {
    //         out << s.first->getName() << " (" << s.second << " ";
    //         switch (s.first->gender) {
    //             case Gender::Male:
    //                 out << "Brother";
    //                 break;
    //             case Gender::Female:
    //                 out << "Sister";
    //                 break;
    //             case Gender::Unknown:
    //                 out << "Sibling";
    //                 break;
    //         }
    //         out << ") ";
    //     }
    //     out << endl;
    // }

    return out;
}

Person::~Person() {}
