// https://github.com/Enderceliik
// Ender CELIK

#include "database.h"
QSqlDatabase database::sqliteDatabase;
database::database() {
    if (!sqliteDatabase.isValid()) {
        sqliteDatabase = QSqlDatabase::addDatabase("QSQLITE");
        sqliteDatabase.setDatabaseName("banking_automation_sqlite_database.db");

        if (!sqliteDatabase.open()) {
            qDebug() << "The database could not be opened.";
        }
    }
}
database::~database() {
    sqliteDatabase.close();
}
void database::initial_database_definition()
{
    QSqlQuery query(sqliteDatabase);
    if (!sqliteDatabase.open()) {
        sqliteDatabase.open();
    }
    query.prepare("CREATE TABLE users ("
                  "userID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "password VARCHAR(16),"
                  "name VARCHAR(50),"
                  "surname VARCHAR(50),"
                  "balance DOUBLE,"
                  "Iban VARCHAR(26),"
                  "userType INT"
                  ");");
    if(query.exec())
    {
        qDebug() << "users table CREATED succesfully!";
    }
    query.prepare("CREATE TABLE transactions ("
                  "transactionID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "transactionDate DATETIME DEFAULT CURRENT_TIMESTAMP,"
                  "amountTransferred DOUBLE,"
                  "senderIban VARCHAR(26),"
                  "receivingPartyIban VARCHAR(26)"
//                  "FOREIGN KEY(senderID) REFERENCES users(userID),"
//                  "FOREIGN KEY(receivingPartyID) REFERENCES users(userID)"
                  ");");
    if(query.exec())
    {
        qDebug() << "transactions table CREATED succesfully!";
    }
    else
    {
        qDebug() << query.lastError().text()  + " transactions";
    }
    queryString = QString("INSERT INTO users (password, "
                          "name, "
                          "surname, "
                          "balance, "
                          "Iban, "
                          "userType) "
                          "values ('%1', '%2', '%3', %4, '%5', %6);")
                      .arg("159*")
                      .arg("Ender")
                      .arg("Çelik")
                      .arg(19500)
                      .arg("TR 29296161")
                      .arg(0);
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table INSERT succesfully!";
    }
    queryString = QString("INSERT INTO users (password, "
                                  "name, "
                                  "surname, "
                                  "balance, "
                                  "Iban, "
                                  "userType) "
                                  "values ('%1', '%2', '%3', %4, '%5', %6);")
                              .arg("159-")
                              .arg("Lokman")
                              .arg("Pehlivan")
                              .arg(42000)
                              .arg("TR 42424242")
                              .arg(0);
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table INSERT 2 succesfully!";
    }
    queryString = QString("INSERT INTO users (password, "
                          "name, "
                          "surname, "
                          "balance, "
                          "Iban, "
                          "userType) "
                          "values ('%1', '%2', '%3', %4, '%5', %6);")
                      .arg("654")
                      .arg("Ahmet")
                      .arg("Alkaç")
                      .arg(22000)
                      .arg("TR 26262929")
                      .arg(0);
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table INSERT 3 succesfully!";
    }
}

QString database::verification(int userID)
{
    if (!sqliteDatabase.isOpen()) {
        sqliteDatabase.open();
    }

    QSqlQuery query(sqliteDatabase);
    queryString = QString("SELECT password FROM users where userID = %1").arg(userID);
    query.prepare(queryString);
    query.exec();
    query.next();
    return query.value(0).toString();
}

QSqlQuery database::interface_info_fetch(int userID)
{
    if (!sqliteDatabase.isOpen()) {
        sqliteDatabase.open();
    }

    QSqlQuery query(sqliteDatabase);
    queryString = QString("SELECT * FROM users WHERE userID = %1").arg(userID);
    query.prepare(queryString);
    query.exec();
    query.next();
    return query;
}

void database::process(QMap<QString,QVariant> interfaceMap)
{
    if (!sqliteDatabase.isOpen()) {
        sqliteDatabase.open();
    }
    QSqlQuery query(sqliteDatabase);
    queryString = QString("UPDATE users SET balance = balance - %1 where userID = %2;").arg(interfaceMap.value("amountTransferred").toDouble()).arg(interfaceMap.value("userID").toInt());
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table insert succesfully!";
    }
    queryString = QString("UPDATE users SET balance = balance + %1 where userID = %2;").arg(interfaceMap.value("amountTransferred").toDouble()).arg(interfaceMap.value("receivingPartyID").toInt());
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table insert succesfully!";
    }

    queryString = QString("INSERT INTO transactions (amountTransferred, senderIban, receivingPartyIban) values(%1, '%2', '%3');").arg(interfaceMap.value("amountTransferred").toDouble())
                      .arg(interfaceMap.value("Iban").toString())
                      .arg(interfaceMap.value("receivingPartyIban").toString());
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table insert succesfully!";
    }
}

QSqlQueryModel* database::database_query(QString parameter)
{
    if (!sqliteDatabase.isOpen()) {
        sqliteDatabase.open();
    }
    queryString = QString("SELECT transactionDate, amountTransferred, senderIban, receivingPartyIban FROM transactions WHERE senderIban = '%1' OR receivingPartyIban = '%2'").arg(parameter, parameter);
    QSqlQueryModel *model  = new QSqlQueryModel();
    model->setQuery(queryString);
    return model;
}

QSqlQuery database::check_iban(QString Iban)
{
    if (!sqliteDatabase.isOpen()) {
        sqliteDatabase.open();
    }
    QSqlQuery query(sqliteDatabase);
    queryString = QString("SELECT userID, name, surname FROM users WHERE Iban= '%1'").arg(Iban);
    query.prepare(queryString);
    query.exec();
    query.next();
    return query;
}
