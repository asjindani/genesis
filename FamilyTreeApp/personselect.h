#ifndef PERSONSELECT_H
#define PERSONSELECT_H

#include <QDialog>

#include "Person.h"
#include "Tree.h"

#include <iostream>
#include <vector>
#include <map>
using namespace std;

namespace Ui {
class PersonSelect;
}

class PersonSelect : public QDialog
{
    Q_OBJECT

public:
    explicit PersonSelect(QWidget *parent = nullptr, map<int, Person*> people = {}, const bool& multiple = false);
    ~PersonSelect();

    vector<Person*> getPersons() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PersonSelect *ui;

    map<int, Person*> people;
    vector<Person*> selected;
};

#endif // PERSONSELECT_H
