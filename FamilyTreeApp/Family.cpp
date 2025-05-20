#include "Family.h"
#include <algorithm>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QVariant>
#include <QMetaType>

// CONSTRUCTOR

Family::Family()
    : id(0)
    , father(nullptr)
    , mother(nullptr)
{}

// DESTRUCTOR

Family::~Family() {}

// MUTATOR

void Family::addChild(Person* child, const Relation& fatherRelation, const Relation& motherRelation) {
    children.push_back({child, fatherRelation, motherRelation});
    // child->addToBirthFamily(this);
}

void Family::setID(const int& id) {
    this->id = id;
}

// GETTERS

int Family::getID() const {
    return id;
}

Person* Family::getFather() const {
    return father;
}

Person* Family::getMother() const {
    return mother;
}

Person* Family::getChild(const int& ID) const {
    return getChildRelation(ID).person;
}

vector<ChildRelation> Family::getChildRelations() const {
    return children;
}

ChildRelation Family::getChildRelation(const int& ID) const {
    for (const auto& childRelation : children)
        if (childRelation.person->getID() == ID)
            return childRelation;
    return {nullptr, Relation::Unknown, Relation::Unknown};
}

// OUTPUT

ostream& operator<<(ostream& out, const Family& family) {
    out << "FAMILY" << endl;
    if (family.father)
        out << "\tFather: " << family.father->getName() << endl;
    if (family.mother)
        out << "\tMother: " << family.mother->getName() << endl;
    if (!family.children.empty())
        for (const auto& childRelation : family.children)
            out << "\tChild: " << childRelation.person->getName() << endl;
    out << endl;
    return out;
}

void Family::setFather(Person* person) {
    father = person;
}

void Family::setMother(Person* person) {
    mother = person;
}

void Family::removeChild(Person* person) {
    for (int i = 0; i < children.size(); i++) {
        const auto& child = children[i];
        if (child.person == person) {
            children.erase(next(children.begin(), i));
            return;
        }
    }
}

void Family::deleteChild(Person* person) {
    removeChild(person);
    delete person;
}

string Family::getRepr() const {
    // string s = "[" + to_string(id) + "]";

    string fatherName = (father ? father->getName() : "");
    string motherName = (mother ? mother->getName() : "");

    if (!fatherName.empty())
        if (!motherName.empty())
            return fatherName + " & " + motherName;
        else
            return fatherName;
    if (!motherName.empty())
        return motherName;
    return "[Family]";
}

bool Family::hasChild(const int& id) {
    for (const auto& child : children)
        if (child.person->getID() == id)
            return true;
    return false;
}

vector<Person*> Family::getChildren() const {
    vector<Person*> result;
    for (const auto& child : children)
        result.push_back(child.person);
    return result;
}

QDate Family::getMarriageDate() const {
    return marriageDate;
}
