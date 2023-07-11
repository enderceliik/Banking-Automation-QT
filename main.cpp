#include "mainwindow.h"
#include <QApplication>
#include "database.h"

int main(int argc, char *argv[])
{
//    database *db = new database();
//    db->initial_database_definition();
//    delete db;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
