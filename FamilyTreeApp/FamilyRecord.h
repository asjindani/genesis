#ifndef FAMILYRECORD_H
#define FAMILYRECORD_H

#include "Enum.h"
#include "Person.h"

class Person;

struct FamilyRecord {
    Person* father;
    Person* mother;
    Relation fatherRelation;
    Relation motherRelation;
    vector<Person*> siblings;
    vector<SiblingType> siblingRelations;
};

#endif // FAMILYRECORD_H
