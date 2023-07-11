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
    void process(QMap<QString, QVariant>);
    QString verification(int userID);
    QSqlQuery interface_info_fetch(int userID);
    QSqlQuery check_iban(QString Iban);
    QSqlQueryModel* database_query(QString parameter);
private:
    static QSqlDatabase sqliteDatabase;
};

#endif // DATABASE_H
