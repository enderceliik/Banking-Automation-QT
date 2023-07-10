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
    ui->customer_identity_lineEdit->setEnabled(false);
    ui->password_lineEdit->setEnabled(false);
    ui->login_button->setEnabled(false);
    ui->statusbar->showMessage("Checking...");
    if( ui->customer_identity_lineEdit->text().isEmpty() || ui->password_lineEdit->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Bilgilerinizin tamamını girdiğinizden emin olun!"); // çevirilecek
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        ui->statusbar->clearMessage();
        ui->customer_identity_lineEdit->setEnabled(true);
        ui->password_lineEdit->setEnabled(true);
        ui->login_button->setEnabled(true);
        return;
    }
    else
    {
        if(database().verification(ui->customer_identity_lineEdit->text().toInt(), ui->password_lineEdit->text()))
        {
            InterFace *interface = new InterFace(ui->customer_identity_lineEdit->text().toInt());
            close();
            interface->show();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Uyarı!");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Bilgilerinizi doğru girdiğinizden emin olun"); // çevirilecek
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            ui->statusbar->showMessage("Something is wrong... Try again..."); // çevrilecek
            ui->statusbar->clearMessage();
            ui->customer_identity_lineEdit->setEnabled(true);
            ui->password_lineEdit->setEnabled(true);
            ui->login_button->setEnabled(true);
        }
    }
}

