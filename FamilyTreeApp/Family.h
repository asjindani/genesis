#ifndef FAMILY_H
#define FAMILY_H

#include <iostream>
using namespace std;

#include <vector>

#include "ChildRelation.h"

#include "Person.h"
#include "Event.h"

#include <QDate>

class Family {
private:
    int id;
    Person* father;
    Person* mother;
    vector<ChildRelation> children;
    // MarriageEvent* marriageEvent;
    QDate marriageDate;

public:
    Family();

    int getID() const;

    void setID(const int& id);

    void addChild(Person*, const Relation& = Relation::Unknown, const Relation& = Relation::Unknown);
    void removeChild(Person* person);
    void deleteChild(Person* person);
    bool hasChild(const int& id);

    friend std::ostream& operator<<(std::ostream& out, const Family& family);

    // SETTERS
    void setFather(Person* person);
    void setMother(Person* person);

    // GETTERS
    Person* getFather() const;
    Person* getMother() const;
    Person* getChild(const int& ID) const;

    ChildRelation getChildRelation(const int& ID) const;
    vector<ChildRelation> getChildRelations() const;

    vector<Person*> getChildren() const;

    string getRepr() const;

    QDate getMarriageDate() const;

    ~Family();
};

#endif // FAMILY_H
