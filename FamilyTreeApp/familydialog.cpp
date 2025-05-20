#include "familydialog.h"
#include "ui_familydialog.h"

#include "personselect.h"

#include <map>

FamilyDialog::FamilyDialog(QWidget *parent, Tree* tree, Family* family)
    : QDialog(parent)
    , ui(new Ui::FamilyDialog)
{
    ui->setupUi(this);
    setWindowTitle("New Family");

    if (family) {
        if (auto father = family->getFather())
            ui->fatherLabel->setText(QString::fromStdString(father->getRepr()));
        if (auto mother = family->getMother())
            ui->motherLabel->setText(QString::fromStdString(mother->getRepr()));

        for (const auto& child : family->getChildRelations()) {
            QListWidgetItem* item = new QListWidgetItem;
            item->setData(Qt::UserRole, child.person->getID());
            item->setText(QString::fromStdString(child.person->getRepr()));
            ui->childrenList->addItem(item);
        }
    }

    this->tree = tree;
    this->family = (family ? family : new Family);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

FamilyDialog::~FamilyDialog()
{
    delete ui;
}

// map<int, Person*> filterPeople(Tree* tree, Family* family, const Gender& gender = Gender::Unknown) {
//     map<int, Person*> filteredPeople;

//     for (const auto& personPair : tree->getPeople()) {
//         Person* person = personPair.second;

//         Person* father = family->getFather();

//         if (person == father)
//             goto nextPerson;

//         Person* mother = family->getMother();

//         if (person == mother)
//             goto nextPerson;

//         if (person->getGender() != gender && gender != Gender::Unknown)
//             goto nextPerson;

//         for (const auto& child : family->getChildren()) {
//             if (person == child)
//                 goto nextPerson;
//         }

//         filteredPeople.insert(personPair);

//         nextPerson:;
//     }

//     return filteredPeople;
// }

map<int, Person*> filterByGender(map<int, Person*> people, const Gender& gender = Gender::Unknown) {
    map<int, Person*> filteredPeople;
    for (const auto& [id, person] : people)
        if (person->getGender() == gender)
            filteredPeople.insert({id, person});
    return filteredPeople;
}

map<int, Person*> filterAlreadyInFamily(map<int, Person*> people, Family* family) {
    if (!family)
        return people;

    map<int, Person*> filteredPeople;

    for (const auto& [id, person] : people) {
        if (person == family->getFather())
            continue;

        if (person == family->getMother())
            continue;

        for (const auto& child : family->getChildren())
            if (person == child)
                goto nextPerson;

        filteredPeople.insert({id, person});
        nextPerson:;
    }
    return filteredPeople;
}

map<int, Person*> filterChildrenByAncestors(map<int, Person*> people, Tree* tree, Family* family) {
    if (!family)
        return people;

    map<int, Person*> children;

    for (const auto& [id, child] : people) {
        for (const auto& parent : {family->getFather(), family->getMother()})
            if (parent)
                if (tree->getAncestors(parent->getID()).count(child))
                        goto nextChild;
        children.insert({id, child});
        nextChild:;
    }
    return children;
}

map<int, Person*> filterParentsByDescendants(map<int, Person*> people, Tree* tree, Family* family) {
    map<int, Person*> parents;
    for (const auto& [id, parent] : people) {
        for (const auto& child : family->getChildren())
            if (tree->getDescendants(child->getID()).count(parent))
                goto nextPerson;
        parents.insert({id, parent});
        nextPerson:;
    }
    return parents;
}

map<int, Person*> filterChildrenByBirthFamily(map<int, Person*> children, Tree* tree) {
    map<int, Person*> filteredChildren;
    for (const auto& [id, child] : children)
        if (!tree->getBirthFamily(child->getID()))
            filteredChildren.insert({id, child});
    return filteredChildren;
}

void FamilyDialog::on_selectFatherButton_clicked()
{
    auto people = tree->getPeople();
    people = filterByGender(people, Gender::Male);
    people = filterAlreadyInFamily(people, family);
    people = filterParentsByDescendants(people, tree, family);

    PersonSelect* dialog = new PersonSelect(this, people);

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
    auto people = tree->getPeople();
    people = filterByGender(people, Gender::Female);
    people = filterAlreadyInFamily(people, family);
    people = filterParentsByDescendants(people, tree, family);

    PersonSelect* dialog = new PersonSelect(this, people);
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
    auto people = tree->getPeople();
    people = filterChildrenByBirthFamily(people, tree);
    people = filterAlreadyInFamily(people, family);
    people = filterChildrenByAncestors(people, tree, family);
    PersonSelect* dialog = new PersonSelect(this, people, true);

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
