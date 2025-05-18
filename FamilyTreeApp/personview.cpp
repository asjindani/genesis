#include "personview.h"
#include "ui_personview.h"

#include "Person.h"

PersonView::PersonView(QWidget *parent, Person* person)
    : QDialog(parent)
    , ui(new Ui::PersonView)
{
    ui->setupUi(this);

    setWindowTitle("Person");
    if (person)
        ui->personName->setText(QString::fromStdString(person->getName()));
}

PersonView::~PersonView()
{
    delete ui;
}
