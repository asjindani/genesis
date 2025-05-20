#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>

#include "Person.h"

Dialog::Dialog(QWidget *parent, Tree* tree, Person* person)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    this->tree = tree;
    ui->setupUi(this);
    setWindowTitle("Person");
    ui->gender->setCurrentIndex(0);

    if (this->person = person) {
        ui->firstName->setText(QString::fromStdString(person->getFirstName()));
        ui->lastName->setText(QString::fromStdString(person->getLastName()));
        ui->gender->setCurrentIndex(person->getGender());
        ui->dateEdit->setDate(person->getBirthDate());
    }

    connect(this, &QDialog::accepted, this, &Dialog::onAccepted);

    // ui->id->setText(QString::number(id));

    // ui->buttonBox->setEnabled(!ui->id->text().isEmpty());

    // connect(ui->id, &QLineEdit::textChanged, this, [this](const QString &text){
    //     ui->buttonBox->setEnabled(!text.isEmpty());
    // });
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onAccepted()
{
    if (!person)
        person = new Person;

    person->setFirstName(ui->firstName->text().toStdString());
    person->setLastName(ui->lastName->text().toStdString());
    person->setGender(ui->gender->currentText().toStdString());
    person->setBirthDate(ui->dateEdit->date());
}

Person* Dialog::getPerson() const {
    return person;
}

bool Dialog::genderConfirmation() {
    string gender = ui->gender->currentText().toStdString();

    if (person) {
        string oldGender = GenderCast::toString(person->getGender());

        if (oldGender != gender)
            if (tree->getMarriedFamilies(person->getID()).size()) {
                QMessageBox messageBox;
                messageBox.setWindowTitle("Confirmation");
                messageBox.setText(
                    "Are you sure you want to change the gender?\n"
                    "This will remove the person as a parent in all families."
                    );
                messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                messageBox.setDefaultButton(QMessageBox::No);

                if (messageBox.exec() == QMessageBox::Yes) {
                    return true;
                } else {
                    return false;
                }
            }
    }
    return true;
}

void Dialog::on_buttonBox_clicked(QAbstractButton *button)
{
}

