#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "Tree.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addToPersonList(Person* person);
    void addToFamilyList(Family* family);

private slots:
    void on_buttonNewPerson_clicked();

    void on_personList_itemDoubleClicked(QListWidgetItem *item);

    void on_buttonNewFamily_clicked();

    void on_removePerson_clicked();

    void on_removeFamily_clicked();

    void refreshFamilyList();
    void refreshPersonList();

private:
    Ui::MainWindow *ui;

    Tree* tree;
    int personID;
    int familyID;
};
#endif // MAINWINDOW_H
