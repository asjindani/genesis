#ifndef PERSON_H
#define PERSON_H


#include <iostream>
#include <chrono>
using namespace std;

#include <map>
#include <vector>
#include <set>

#include "Enum.h"
#include "Family.h"
#include "FamilyRecord.h"

typedef std::chrono::year_month_day Date;

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
    Date birthDate;

    // pair<Person*, Person*> biologicalParents;

    // Association
    vector<Family*> birthFamilies;
    vector<Family*> marriedFamilies;

public:

    Person();

    int getID() const;
    string getFirstName() const;
    string getLastName() const;
    Gender getGender() const;
    Date getBirthDate() const;

    void setID(const int& id);
    void setFirstName(const string& name);
    void setLastName(const string& name);
    void setGender(const Gender& gender);
    void setBirthDate(const Date& date);

    string getName() const;
    string getRepr() const;

    set<Person*> getDescendants() const;

    // void setParents(Person* father, Person* mother, const Relation&);
    // pair<Person*, Person*> getParents(const Relation&) const;

    void addToBirthFamily(Family*);
    void addToMarriedFamily(Family*);

    void removeBirthFamily(Family* family);
    void removeMarriedFamily(Family* family);

    vector<pair<ParentRelation, ParentRelation>> getParentRelations() const;
    vector<vector<SiblingRelation>> getSiblingRelations() const;

    vector<FamilyRecord> getBirthFamilyRecords() const;
    vector<Family*> getMarriedFamilyRecords() const;

    friend ostream& operator<<(ostream&, const Person&);

    // ~Person();
};

#endif // PERSON_H
