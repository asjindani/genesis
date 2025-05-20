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
    MainWindow(QWidget *parent = nullptr, Tree* tree = nullptr);
    ~MainWindow();
    Tree* tree;

private slots:
    void on_buttonNewPerson_clicked();

    void on_personList_itemDoubleClicked(QListWidgetItem *item);

    void on_buttonNewFamily_clicked();

    void on_removePerson_clicked();

    void on_removeFamily_clicked();

    void on_familyList_itemDoubleClicked(QListWidgetItem *item);

    void on_actionNewTree_triggered();

    void on_actionSaveTree_triggered();

    void on_actionLoadTree_triggered();

    void on_actionCloseTree_triggered();

    void on_selectHomePerson_clicked();

    void on_visualizeTree_clicked();

private:
    Ui::MainWindow *ui;

    int personID;
    int familyID;

    void addToPersonList(Person* person);
    void addToFamilyList(Family* family);

    void refreshFamilyList();
    void refreshPersonList();

    void updateListItem(QListWidget* list, const int& id, const string& text);

};
#endif // MAINWINDOW_H
