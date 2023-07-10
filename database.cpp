#include "database.h"

database::database() {
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("mydatabase.db");

    if (!database.open()) {
        qDebug() << "Veritabanı açılamadı.";
    }
}
database::~database() {
    database.close();
}
void database::initial_database_definition()
{
    if (!database.open()) {
        return;
    }
    QSqlQuery query(database);

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
                      .arg(1600.0)
                      .arg("TR 296143")
                      .arg(0);
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
                              .arg(2600.75)
                              .arg("TR 424242")
                              .arg(0);
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "users table INSERT 2 succesfully!";
    }

}
bool database::verification(int userID, QString password)
{
    if (!database.isOpen()) {
        database.open();
    }
    QSqlQuery query(database);
    queryString = QString("SELECT password FROM users where userID = '%1'").arg(userID);
    query.exec();
    query.next();
    QSettings settings("banking_automation.ini", QSettings::IniFormat);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(password == query.value(0).toString())
    {

        QString logMessage = QString::number(userID);

        settings.beginGroup("success logins");
        settings.setValue(timestamp, logMessage);
        settings.endGroup();
        return true;
    } else
    {
       QSettings settings("banking_automation.ini", QSettings::IniFormat);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString logMessage = QString::number(userID);

        settings.beginGroup("failed logins");
        settings.setValue(timestamp, logMessage);
        settings.endGroup();
        return false;
    }
}
QMap<QString, QVariant> database::interface_info_fetch(int userID)
{
    QMap<QString, QVariant> interfaceInfoMap;
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE userID = :userID");
    query.bindValue(":userID", userID);
    query.exec();
    query.next();
    interfaceInfoMap.insert("userID",query.value(0).toInt());
    interfaceInfoMap.insert("password",query.value(1).toString());
    interfaceInfoMap.insert("name",query.value(2).toString());
    interfaceInfoMap.insert("surname",query.value(3).toString());
    interfaceInfoMap.insert("balance",query.value(4).toDouble());
    interfaceInfoMap.insert("Iban",query.value(5).toString());
    interfaceInfoMap.insert("userType",query.value(6).toString());
    return interfaceInfoMap;
}

void database::process(QMap<QString,QVariant> interfaceMap)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET balance = balance - :amountTransferred where userID = :senderID;");
    query.bindValue(":amountTransferred", interfaceMap.value("amountTransferred").toDouble());
    query.bindValue(":senderID", interfaceMap.value("userID").toString());
    if(query.exec())
    {
        qDebug() << "users table insert succesfully!";
    }
    query.prepare("UPDATE users SET balance = balance + :amountTransferred where userID = :receivingPartyID;");
    query.bindValue(":amountTransferred", interfaceMap.value("amountTransferred").toDouble());
    query.bindValue(":receivingPartyID", interfaceMap.value("receivingPartyID").toString());
    if(query.exec())
    {
        qDebug() << "users table insert succesfully!";
    }


    query.prepare("INSERT INTO transactions (amountTransferred, senderIban, receivingPartyIban) values(:amountTransferred, :senderIban, :receivingPartyIban);");
    query.bindValue(":amountTransferred", interfaceMap.value("amountTransferred").toDouble());
    query.bindValue(":senderIban", interfaceMap.value("Iban").toString());
    query.bindValue(":receivingPartyIban", interfaceMap.value("receivingPartyIban").toString());
    if(query.exec())
    {
        qDebug() << "users table insert succesfully!";
    }
    double totalyBalance = interfaceMap.value("balance").toDouble() - interfaceMap.value("amountTransferred").toDouble();
    QSettings settings("banking_automation.ini", QSettings::IniFormat);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logMessage = interfaceMap.value("amountTransferred").toString() + " " + interfaceMap.value("Iban").toString() + " " + interfaceMap.value("receivingPartyIban").toString() + " " + QString::number(totalyBalance);

    settings.beginGroup("money transfers");
    settings.setValue(timestamp, logMessage);
    settings.endGroup();
    int i=5;
    QString str=QString("%1").arg(i);
}

QSqlQueryModel* database::database_query(QString parameter)
{
    QString query = QString("SELECT * FROM transactions where senderIban = '%1' or receivingPartyIban = '%2'").arg(parameter, parameter);
    QSqlQueryModel *model  = new QSqlQueryModel();
    model->setQuery(query);
    return model;
}
