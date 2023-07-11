#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>

namespace Ui {
class InterFace;
}

class InterFace : public QMainWindow
{
    Q_OBJECT

public:
    explicit InterFace(const int userID, QWidget *parent = nullptr);
    ~InterFace();
    QMap<QString, QVariant> interfaceInfoMap;
    int userID;

private slots:
    void on_send_pushButton_clicked();
    void load_interface(int userID);
    bool check_value();
    bool check_iban();
    void on_amount_lineEdit_editingFinished();
    void on_Iban_lineEdit_editingFinished();
    void on_exit_pushButton_clicked();

private:
    Ui::InterFace *ui;
};

#endif // INTERFACE_H
