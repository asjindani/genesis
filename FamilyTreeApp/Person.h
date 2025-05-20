#ifndef PERSON_H
#define PERSON_H


#include <iostream>
using namespace std;

#include <map>
#include <vector>
#include <set>

#include "Enum.h"
#include "Family.h"
#include "FamilyRecord.h"

#include <QDate>

class Family;
class Person;

using ParentRelation = pair<Person*, Relation>;
using SiblingRelation = pair<Person*, SiblingType>;

class Person {
private:
    int id;

    // Information
    string firstName;
    string lastName;
    Gender gender;
    QDate birthDate;

public:

    Person();

    int getID() const;
    string getFirstName() const;
    string getLastName() const;
    Gender getGender() const;
    QDate getBirthDate() const;

    void setID(const int& id);
    void setFirstName(const string& name);
    void setLastName(const string& name);
    void setGender(const Gender& gender);
    void setGender(const string& gender);
    void setBirthDate(const QDate& date);

    string getName() const;
    string getRepr() const;

    vector<pair<ParentRelation, ParentRelation>> getParentRelations() const;
    vector<vector<SiblingRelation>> getSiblingRelations() const;

    friend ostream& operator<<(ostream&, const Person&);

    ~Person();
};

#endif // PERSON_H
