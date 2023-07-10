#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QMap>
class database
{
public:
    database();
    ~database();
    QString queryString;
    void initial_database_definition();
    QMap<QString, QVariant> interface_info_fetch(int userID);
    bool verification(int userID, QString password);
    void process(QMap<QString, QVariant>);
    QSqlQueryModel* database_query(QString parameter);

private:
    QSqlDatabase database;
};

#endif // DATABASE_H
