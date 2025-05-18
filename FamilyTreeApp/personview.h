#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QDialog>

#include "Person.h"

namespace Ui {
class PersonView;
}

class PersonView : public QDialog
{
    Q_OBJECT

public:
    explicit PersonView(QWidget *parent = nullptr, Person* person = nullptr);
    ~PersonView();

private:
    Ui::PersonView *ui;
};

#endif // PERSONVIEW_H
