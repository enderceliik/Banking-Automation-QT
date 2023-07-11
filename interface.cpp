#include "interface.h"
#include "ui_interface.h"
#include "database.h"
#include "mainwindow.h"

InterFace::InterFace(const int userID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InterFace)
{    
    this->userID = userID;
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    load_interface(this->userID);
}

InterFace::~InterFace()
{
    delete ui;
}

void InterFace::load_interface(int userID)
{
    database* db = new database();
    QSqlQuery query = db->interface_info_fetch(userID);
    QSqlQueryModel *model = db->database_query(query.value(5).toString()); // Iban info send the func.
    delete db;
    interfaceInfoMap.insert("userID",query.value(0).toInt());
    interfaceInfoMap.insert("password",query.value(1).toString());
    interfaceInfoMap.insert("name",query.value(2).toString());
    interfaceInfoMap.insert("surname",query.value(3).toString());
    interfaceInfoMap.insert("balance",query.value(4).toDouble());
    interfaceInfoMap.insert("Iban",query.value(5).toString());
    interfaceInfoMap.insert("userType",query.value(6).toString());

    ui->balance_label->setText(interfaceInfoMap.value("balance").toString());
    ui->Iban_label->setText(interfaceInfoMap.value("Iban").toString());
    ui->name_surname_label->setText(interfaceInfoMap.value("name").toString()+" "+interfaceInfoMap.value("surname").toString());

    ui->tableView->setModel(model);
    ui->tableView->show();
    ui->tableView->update();
}

void InterFace::on_send_pushButton_clicked()
{
    ui->Iban_lineEdit->setEnabled(false);
    ui->amount_lineEdit->setEnabled(false);
    ui->exit_pushButton->setEnabled(false);
    interfaceInfoMap.insert("amountTransferred",ui->amount_lineEdit->text().toDouble());
    interfaceInfoMap.insert("receivingPartyIban", ui->Iban_lineEdit->text());
    if(check_iban() && check_value())
    {
        database* db = new database();
        db->process(interfaceInfoMap);
        delete db;
        double totalyBalance = interfaceInfoMap.value("balance").toDouble() - interfaceInfoMap.value("amountTransferred").toDouble();

        QSettings settings("banking_automation.ini", QSettings::IniFormat);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString logMessage = interfaceInfoMap.value("amountTransferred").toString() + " " + interfaceInfoMap.value("Iban").toString() + " " + interfaceInfoMap.value("receivingPartyIban").toString() + " " + QString::number(totalyBalance);
        settings.beginGroup("money transfers transfer_amount sender receiving party remaining account");
        settings.setValue(timestamp, logMessage);
        settings.endGroup();

    }
    ui->Iban_lineEdit->setEnabled(true);
    ui->amount_lineEdit->setEnabled(true);
    ui->exit_pushButton->setEnabled(true);
    load_interface(this->userID);
}

bool InterFace::check_value()
{
    bool succes;
    double doubleValue = ui->amount_lineEdit->text().toDouble(&succes);
    if(succes)
    {
        if(doubleValue > interfaceInfoMap.value("balance").toDouble() || doubleValue == 0 && doubleValue < 0)
        {
            return false;
        } else
        {
            return true;
        }
    }
    else
        return false;

}

bool InterFace::check_iban()
{
    database* db = new database();
    QSqlQuery query = db->check_iban(ui->Iban_lineEdit->text());
    delete db;
    if(!query.value(1).toString().isEmpty() && !query.value(2).toString().isEmpty())
    {
        interfaceInfoMap.insert("receivingPartyID", query.value(0).toInt());
        interfaceInfoMap.insert("receivingName", query.value(1).toString());
        interfaceInfoMap.insert("receivingSurname", query.value(2).toString());
        return true;
    }
    else
        return false;
}

void InterFace::on_Iban_lineEdit_editingFinished()
{
    if(ui->Iban_lineEdit->text().isEmpty() || ui->Iban_lineEdit->text() == interfaceInfoMap.value("Iban").toString())
    {
        ui->receiver_name_label->clear();
        ui->send_pushButton->setEnabled(false);
        ui->amount_lineEdit->setEnabled(false);
    }
    else
    {
        ui->Iban_lineEdit->setEnabled(false);
        if(!check_iban())
        {
            ui->send_pushButton->setEnabled(false);
            ui->amount_lineEdit->setEnabled(false);
            ui->receiver_name_label->clear();
        }
        else
        {
            ui->amount_lineEdit->setEnabled(true); 
            ui->receiver_name_label->setText(interfaceInfoMap.value("receivingName").toString() + " " + interfaceInfoMap.value("receivingSurname").toString());
        }
        ui->Iban_lineEdit->setEnabled(true);
    }
}


void InterFace::on_amount_lineEdit_editingFinished()
{
    check_value() ?ui->send_pushButton->setEnabled(true): ui->send_pushButton->setEnabled(false);
}


void InterFace::on_exit_pushButton_clicked()
{
    MainWindow *mainWindow = new MainWindow();
    interfaceInfoMap.clear();
    close();
    mainWindow->show();
}

