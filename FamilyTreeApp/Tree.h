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
        int id;
        string name;

        Person* homePerson;
        map<int, Person*> people;
        map<int, Family*> families;

        QSqlDatabase db;

    public:
        Tree();

        int getID() const;
        void setID(const int& id);

        void setHomePerson(Person* person);
        Person* getHomePerson() const;

        void addPerson(Person* person);
        void deletePerson(const int& id);
        Person* getPerson(const int& id);

        void addFamily(Family* family);
        void deleteFamily(const int& id);
        Family* getFamily(const int& id);

        bool personExists(const int& id) const;
        bool familyExists(const int& id) const;

        map<int, Person*> getPeople() const;
        map<int, Family*> getFamilies() const;

        // Connect to database
        void connect(const string& path);

        // Insert into database
        bool insertPerson(Person* person);
        bool insertFamily(Family* person);

        // Update into database
        void updatePerson(const int& id);
        void updateFamily(const int& id);

        // Load from databsae
        void load();
        void autosave();

        void saveFile(const string& path);
        void loadFile(const string& path);

        vector<Person*> getChildren(const int& id);

        set<Person*> getAncestors(const int& id);
        set<Person*> getDescendants(const int& id);

        Family* getBirthFamily(const int& personID);
        set<Family*> getMarriedFamilies(const int& personID);

        void clear();
        bool empty() const;

        ~Tree();

    private:
        set<Family*> getBirthFamilies(const int& personID);
};

#endif // TREE_H
