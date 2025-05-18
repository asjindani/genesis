#include "Family.h"
#include <algorithm>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

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
    child->addToBirthFamily(this);
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
    throw runtime_error("Child not found in the family");
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
    if (father)
        father->removeMarriedFamily(this);

    father = person;
    if (father) {
        if (father->getGender() != Gender::Male)
            throw runtime_error("Father must be male!");
        father->addToMarriedFamily(this);
    }
}

void Family::setMother(Person* person) {
    if (mother) mother->removeMarriedFamily(this);

    mother = person;
    if (mother) {
        if (mother->getGender() != Gender::Female)
            throw runtime_error("Mother must be female!");
        mother->addToMarriedFamily(this);
    }
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
    string s = "[" + to_string(id) + "]";

    string fatherName = (father ? father->getName() : "");
    string motherName = (mother ? mother->getName() : "");

    if (!fatherName.empty())
        if (!motherName.empty())
            return s + ' ' + fatherName + " & " + motherName;
        else
            return s + ' ' + fatherName;
    if (!motherName.empty())
        return s + ' ' + motherName;
    return s;
}
