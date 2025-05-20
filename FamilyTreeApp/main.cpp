#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Tree tree;
    tree.connect("genesis.db");
    tree.load();

    MainWindow w(nullptr, &tree);

    w.setWindowTitle("Genesis");
    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();

    return a.exec();
}
