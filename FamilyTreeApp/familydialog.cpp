#include "familydialog.h"
#include "ui_familydialog.h"

#include "Tree.h"
#include "personselect.h"

#include <map>

FamilyDialog::FamilyDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FamilyDialog)
{
    ui->setupUi(this);
    setWindowTitle("New Family");

    family = new Family;

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

FamilyDialog::~FamilyDialog()
{
    delete ui;
}

void FamilyDialog::setTree(Tree* tree) {
    this->tree = tree;
}

map<int, Person*> filterPeople(Tree* tree, Family* family, const Gender& gender = Gender::Unknown) {
    map<int, Person*> filteredPeople;

    for (const auto& personPair : tree->getPeople()) {
        Person* person = personPair.second;
        if (person != family->getFather() && person != family->getMother()
        && (person->getGender() == gender || gender == Gender::Unknown)) {

            for (const auto& child : family->getChildRelations())
                if (person == child.person) goto nextPerson;

            filteredPeople.insert(personPair);
        }
        nextPerson:;
    }

    return filteredPeople;
}

void FamilyDialog::on_selectFatherButton_clicked()
{

    PersonSelect* dialog = new PersonSelect(this, filterPeople(tree, family, Gender::Male));

    dialog->show();
    int result = dialog->exec();

    if (result == QDialog::Accepted) {
        auto persons = dialog->getPersons();
        Person* person = persons.empty() ? nullptr : persons[0];
        QString text = "None";
        if (person)
            text = QString::fromStdString(person->getRepr());
        ui->fatherLabel->setText(text);
        family->setFather(person);
    }
}

void FamilyDialog::on_selectMotherButton_clicked()
{
    PersonSelect* dialog = new PersonSelect(this, filterPeople(tree, family, Gender::Female));
    dialog->show();
    int result = dialog->exec();

    if (result == QDialog::Accepted) {
        auto persons = dialog->getPersons();
        Person* person = persons.empty() ? nullptr : persons[0];
        QString text = "None";
        if (person)
            text = QString::fromStdString(person->getRepr());
        ui->motherLabel->setText(text);
        family->setMother(person);
    }
}


void FamilyDialog::on_addChildrenButton_clicked()
{
    PersonSelect* dialog = new PersonSelect(this, filterPeople(tree, family), true);

    dialog->show();

    int result = dialog->exec();

    if (result == QDialog::Accepted) {
        for (const auto& person : dialog->getPersons()) {
            QListWidgetItem* item = new QListWidgetItem;
            item->setData(Qt::UserRole, person->getID());
            item->setText(QString::fromStdString(person->getRepr()));
            ui->childrenList->addItem(item);
            family->addChild(person);
        }
    }

    delete dialog;
}


void FamilyDialog::on_removeChildren_clicked()
{
    for (auto& item : ui->childrenList->selectedItems()) {
        int id = item->data(Qt::UserRole).toInt();
        family->removeChild(family->getChild(id));
        delete item;
        ui->childrenList->setCurrentRow(0);
    }
}

Family* FamilyDialog::getFamily() const {
    return family;
}
