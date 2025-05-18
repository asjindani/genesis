#include "Enum.h"

#include <iostream>
using namespace std;

string GenderCast::toString(const Gender& gender) {
    switch (gender) {
    case Gender::Male:
        return "Male";
    case Gender::Female:
        return "Female";
    default:
        return "Unknown";
    }
}

Gender GenderCast::fromString(const string& str) {
    if (str == "Male")
        return Gender::Male;
    if (str == "Female")
        return Gender::Female;
    return Gender::Unknown;
}
