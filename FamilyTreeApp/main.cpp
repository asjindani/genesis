#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void setupDatabase();

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;

    w.setWindowTitle("Genealogy App");
    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();

    return a.exec();
}
