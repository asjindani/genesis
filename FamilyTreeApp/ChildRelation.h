#ifndef CHILDRELATION_H
#define CHILDRELATION_H

#include "Person.h"

class Person;
enum class Relation;

struct ChildRelation {
    Person* person;
    Relation fatherRelation;
    Relation motherRelation;

    ChildRelation(Person* child, const Relation& fatherRelation, const Relation& motherRelation) : person(child), fatherRelation(fatherRelation), motherRelation(motherRelation) {}
};

#endif // CHILDRELATION_H
