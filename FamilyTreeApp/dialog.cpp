#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QLineEdit>
#include <QDialogButtonBox>

#include "Person.h"

Dialog::Dialog(QWidget *parent, Person* person)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("New Person");
    ui->gender->setCurrentIndex(0);

    if (person) {
        ui->firstName->setText(QString::fromStdString(person->getFirstName()));
        ui->lastName->setText(QString::fromStdString(person->getLastName()));
        ui->gender->setCurrentIndex(person->getGender());
        this->person = person;
    } else {
        this->person = new Person;
    }

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

void Dialog::on_buttonBox_accepted()
{
    person->setFirstName(ui->firstName->text().toStdString());
    person->setLastName(ui->lastName->text().toStdString());
    person->setGender(GenderCast::fromString(ui->gender->currentText().toStdString()));
}

// QString Dialog::getFirstName() const
// {
//     return firstName;
// }
// QString Dialog::getLastName() const
// {
//     return lastName;
// }
// QString Dialog::getGender() const
// {
//     return gender;
// }
Person* Dialog::getPerson() const {
    return person;
}
