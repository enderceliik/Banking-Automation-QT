#include "mainwindow.h"
#include <QApplication>
#include "database.h"

int main(int argc, char *argv[])
{
    database().initial_database_definition();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
