#include "personselect.h"
#include "ui_personselect.h"

#include <QMessageBox>

#include <iostream>
#include <map>

using namespace std;

PersonSelect::PersonSelect(QWidget *parent, map<int, Person*> people, const bool& multiple)
    : QDialog(parent)
    , ui(new Ui::PersonSelect)
    , people(people)
{
    ui->setupUi(this);

    ui->personList->setSelectionMode(multiple ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);

    for (const auto& personPair : people) {
        int id = personPair.first;
        Person* person = personPair.second;
        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::UserRole, id);
        item->setText(QString::fromStdString(person->getRepr()));
        ui->personList->addItem(item);
    }
}

vector<Person*> PersonSelect::getPersons() const {
    return selected;
}

PersonSelect::~PersonSelect()
{
    delete ui;
}

void PersonSelect::on_buttonBox_accepted()
{
    for (const auto& item : ui->personList->selectedItems()) {
        int id = item->data(Qt::UserRole).toInt();
        selected.push_back(people[id]);
    }
}

