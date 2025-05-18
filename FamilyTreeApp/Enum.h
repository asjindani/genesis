#ifndef ENUM_H
#define ENUM_H

#include <iostream>
using namespace std;

enum class Relation {
    Unknown,
    Biological,
    Foster,
    Adoptive
};

enum class SiblingType {
    Full,
    Half,
    Step
};

enum Gender {
    Unknown,
    Male,
    Female
};

class GenderCast {
    public:
        static string toString(const Gender&);
        static Gender fromString(const string&);
};

#endif // ENUM_H
