#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./dialog.h"
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

#include "Tree.h"
#include "Person.h"
#include "familydialog.h"
#include "personview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tree = new Tree;

    tree->setupDatabase("genealogy.db");
    tree->load();

    refreshPersonList();
    refreshFamilyList();
}

void MainWindow::refreshPersonList() {
    ui->personList->clear();
    for (const auto& personPair : tree->getPeople())
        addToPersonList(personPair.second);
}

void MainWindow::refreshFamilyList() {
    ui->familyList->clear();
    for (const auto& familyPair : tree->getFamilies())
        addToFamilyList(familyPair.second);
}

void MainWindow::addToPersonList(Person* person) {
    QListWidgetItem* item = new QListWidgetItem;
    item->setData(Qt::UserRole, person->getID());
    item->setText(QString::fromStdString(person->getRepr()));
    ui->personList->addItem(item);
}

void MainWindow::addToFamilyList(Family* family) {
    QListWidgetItem* item = new QListWidgetItem;
    item->setData(Qt::UserRole, family->getID());
    item->setText(QString::fromStdString(family->getRepr()));
    ui->familyList->addItem(item);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonNewPerson_clicked()
{
    Dialog* dialog = new Dialog(this);
    dialog->show();

    if (dialog->exec() == QDialog::Accepted) {
        Person* person = dialog->getPerson();
        tree->addPerson(person);
        addToPersonList(person);
    }

    delete dialog;
}


void MainWindow::on_personList_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();

    if (Person* person = tree->getPerson(id)) {
        Dialog* dialog = new Dialog(this, person);
        dialog->show();
        if (dialog->exec() == QDialog::Accepted) {
            tree->updatePersonInDatabase(id);
            refreshPersonList();
            refreshFamilyList();
        }
        delete dialog;
        // PersonView* view = new PersonView(this, person);
        // view->show();

        // int count = person->getDescendants().size();
        // QMessageBox::information(this, "Hi", QString::number(count));
    }
}


void MainWindow::on_buttonNewFamily_clicked()
{
    FamilyDialog* dialog = new FamilyDialog(this);

    dialog->show();
    dialog->setTree(tree);

    if (dialog->exec() == QDialog::Accepted) {
        Family* family = dialog->getFamily();

        tree->addFamily(family);
        addToFamilyList(family);
    } else {
        delete dialog->getFamily();
    }

    delete dialog;
}


void MainWindow::on_removePerson_clicked()
{
    for (auto& item : ui->personList->selectedItems()) {
        // auto index = ui->personList->indexFromItem(item);
        int id = item->data(Qt::UserRole).toInt();
        tree->deletePerson(id);
        delete item;
        ui->personList->setCurrentRow(0);
    }

    refreshFamilyList();
}


void MainWindow::on_removeFamily_clicked()
{
    for (auto& item : ui->familyList->selectedItems()) {
        int id = item->data(Qt::UserRole).toInt();
        tree->deleteFamily(id);
        delete item;
        ui->familyList->setCurrentRow(0);
    }
}

