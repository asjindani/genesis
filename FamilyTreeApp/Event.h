#ifndef EVENT_H
#define EVENT_H

#include <iostream>
using namespace std;

class Event {
protected:
    string location;
    string description;

public:
    Event();
    ~Event();
};

class MarriageEvent : public Event {
protected:
    Person* groom;
    Person* bride;

};

#endif // EVENT_H
