#ifndef TREE_H
#define TREE_H

#include <iostream>
using namespace std;

#include <map>
#include <vector>

#include <QSqlDatabase>

#include "Person.h"
#include "Family.h"

class Tree {
private:
    Person* homePerson;
    map<int, Person*> people;
    map<int, Family*> families;

    QSqlDatabase db;

public:
    Tree();

    void setHomePerson(Person* person);
    Person* getHomePerson() const;
    void addPerson(Person* person);
    void deletePerson(const int& id);
    Person* getPerson(const int& id);
    void addFamily(Family* family);
    void deleteFamily(const int& id);
    // Family* createFamily(Person* father = nullptr, Person* mother = nullptr);
    map<int, Person*> getPeople() const;
    map<int, Family*> getFamilies() const;

    void updatePersonInDatabase(const int& id);

    void setupDatabase(const string& path);
    void load();

    void clear();
};

#endif // TREE_H
