#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "interface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_login_button_clicked()
{
    database db;
    ui->customer_identity_lineEdit->setEnabled(false);
    ui->password_lineEdit->setEnabled(false);
    ui->login_button->setEnabled(false);

    ui->statusbar->showMessage("Checking...");

    QMessageBox msgBox;
    msgBox.setWindowTitle("Warning");
    msgBox.setText("Bilgilerinizin tamamını girdiğinizden emin olun!");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    QSettings settings("banking_automation.ini", QSettings::IniFormat);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logMessage = QString::number(userID);

    if(ui->customer_identity_lineEdit->text().isEmpty() || ui->password_lineEdit->text().isEmpty())
    {
        msgBox.exec();
        ui->statusbar->clearMessage();
        ui->customer_identity_lineEdit->setEnabled(true);
        ui->password_lineEdit->setEnabled(true);
        ui->login_button->setEnabled(true);
        return;
    }
    else
    {
        QString response = db.verification(ui->customer_identity_lineEdit->text().toInt());
        if(response == ui->password_lineEdit->text())
        {
            settings.beginGroup("success logins");
            settings.setValue(timestamp, logMessage);
            settings.endGroup();
            InterFace *interface = new InterFace(ui->customer_identity_lineEdit->text().toInt());
            close();
            interface->show();
        }
        else
        {
            settings.beginGroup("failed logins");
            settings.setValue(timestamp, logMessage);
            settings.endGroup();
            msgBox.exec();
            ui->statusbar->showMessage("Something is wrong... Try again..."); // çevrilecek
            ui->statusbar->clearMessage();
            ui->customer_identity_lineEdit->setEnabled(true);
            ui->password_lineEdit->setEnabled(true);
            ui->login_button->setEnabled(true);
        }
    }
}

