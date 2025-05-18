#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>

#include "Person.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, Person* person = nullptr);
    QString getFirstName() const;
    QString getLastName() const;
    QString getGender() const;
    ~Dialog();

    Person* getPerson() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    // QString firstName;
    // QString lastName;
    // QString gender;
    Person* person;

    QLineEdit* firstNameTextbox;
};

#endif // DIALOG_H
