#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>

#include "Tree.h"
#include "Person.h"
#include "familydialog.h"
#include "personselect.h"
#include "personview.h"
#include "treedialog.h"

MainWindow::MainWindow(QWidget *parent, Tree* tree)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!tree)
        tree = new Tree;

    this->tree = tree;

    refreshPersonList();
    refreshFamilyList();

    ui->centralwidget->setLayout(ui->horizontalLayout);
}

void MainWindow::updateListItem(QListWidget* list, const int& id, const string& text) {
    for (int i = 0; i < list->count(); i++)
        if (QListWidgetItem* item = list->item(i))
            if (item->data(Qt::UserRole).toInt() == id) {
                item->setText(QString::fromStdString(text));
                return;
            }
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

void MainWindow::on_buttonNewFamily_clicked()
{
    FamilyDialog* dialog = new FamilyDialog(this, tree);
    dialog->show();

    if (dialog->exec() == QDialog::Accepted) {
        Family* family = dialog->getFamily();

        tree->addFamily(family);
        addToFamilyList(family);
    } else {
        delete dialog->getFamily();
    }

    delete dialog;
}


void MainWindow::on_personList_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();

    if (Person* person = tree->getPerson(id)) {

        PersonView* view = new PersonView(this, tree, person);
        view->show();
        view->exec();

        if (view->isAccepted()) {
            updateListItem(ui->personList, id, person->getRepr());
            // refreshPersonList();
            refreshFamilyList();
        }

        delete view;



        // qDebug() << "Ancestors of " << person->getRepr();
        // for (const auto& ancestor : tree->getAncestors(id))
        //     qDebug() << ancestor->getRepr();
        // qDebug() << "Descendants of " << person->getRepr();
        // for (const auto& descendant : tree->getDescendants(id))
        //     qDebug() << descendant->getRepr();

        // Dialog* dialog = new Dialog(this, tree, person);
        // dialog->show();
        // if (dialog->exec() == QDialog::Accepted) {
        //     tree->updatePerson(id);
        //     qDebug() << person->getRepr() << person->getID();
        //     updateListItem(ui->personList, id, person->getRepr());
        //     // refreshPersonList();
        //     refreshFamilyList();
        // }
        // delete dialog;
    }
}


void MainWindow::on_familyList_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();

    if (Family* family = tree->getFamily(id)) {
        FamilyDialog* dialog = new FamilyDialog(this, tree, family);
        dialog->show();
        if (dialog->exec() == QDialog::Accepted) {
            tree->updateFamily(id);
            updateListItem(ui->familyList, id, family->getRepr());
        }
        delete dialog;
    }
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

void MainWindow::on_actionNewTree_triggered()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(
        this, "Save Progress",
        tr("Do you want to save the tree?\n"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        tree->autosave();
    }

}

void MainWindow::on_actionSaveTree_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Family Tree", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    tree->saveFile(filePath.toStdString());
    QMessageBox::information(this, "Success", "Family tree saved successfully.");
}

void MainWindow::on_actionLoadTree_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load Family Tree", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    tree->clear();

    tree->loadFile(filePath.toStdString());
    QMessageBox::information(this, "Success", "Family tree loaded successfully.");

    for (const auto& [id, person] : tree->getPeople())
        qDebug() << person->getRepr();

    tree->autosave();

    refreshPersonList();
    refreshFamilyList();
}

void MainWindow::on_actionCloseTree_triggered() {
    tree->clear();
    refreshPersonList();
    refreshFamilyList();
}

void MainWindow::on_selectHomePerson_clicked()
{
    PersonSelect* dialog = new PersonSelect(this, tree->getPeople());
    dialog->show();
    int result = dialog->exec();

    if (result == QDialog::Accepted) {
        auto persons = dialog->getPersons();
        Person* person = persons.empty() ? nullptr : persons[0];
        QString text = "None";
        if (person)
            text = QString::fromStdString(person->getRepr());
        ui->homePersonLabel->setText(text);
        tree->setHomePerson(person);
    }
}


void MainWindow::on_visualizeTree_clicked()
{
    if (const auto& person = tree->getHomePerson()) {
        TreeDialog* dialog = new TreeDialog(tree, person, this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->showMaximized();
    } else {
        QMessageBox::information(this, "Home Person", "Select home person first!");
    }
}
