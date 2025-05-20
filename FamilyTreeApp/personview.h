#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QDialog>

#include "Person.h"
#include "Tree.h"

namespace Ui {
class PersonView;
}

class PersonView : public QDialog
{
    Q_OBJECT

public:
    explicit PersonView(QWidget *parent = nullptr, Tree* tree = nullptr, Person* person = nullptr);
    ~PersonView();

    bool isAccepted() const;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PersonView *ui;
    Tree* tree;
    Person* person;
    bool accepted;

    void updateView();
};

#endif // PERSONVIEW_H
