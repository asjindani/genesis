#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QAbstractButton>

#include "Person.h"
#include "Tree.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, Tree* tree = nullptr, Person* person = nullptr);
    ~Dialog();

    Person* getPerson() const;

private slots:
    // void on_buttonBox_accepted();

    void on_buttonBox_clicked(QAbstractButton *button);

    void onAccepted();

private:
    Ui::Dialog *ui;
    Person* person;
    Tree* tree;

    QLineEdit* firstNameTextbox;

    bool genderConfirmation();

public Q_SLOTS:
    void accept() override {
        if (genderConfirmation())
            QDialog::accept();
    }
};

#endif // DIALOG_H
