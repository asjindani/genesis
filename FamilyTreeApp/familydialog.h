#ifndef FAMILYDIALOG_H
#define FAMILYDIALOG_H

#include <QDialog>
#include "Tree.h"

namespace Ui {
class FamilyDialog;
}

class FamilyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FamilyDialog(QWidget *parent = nullptr, Tree* tree = nullptr, Family* family = nullptr);
    ~FamilyDialog();

    Family* getFamily() const;

private slots:
    void on_selectFatherButton_clicked();

    void on_selectMotherButton_clicked();

    void on_addChildrenButton_clicked();

    void on_removeChildren_clicked();

private:
    Ui::FamilyDialog *ui;

    Tree* tree;
    Family* family;
};

#endif // FAMILYDIALOG_H
