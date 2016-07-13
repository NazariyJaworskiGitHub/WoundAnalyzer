#include "databaseconnectionedititemwidget.h"
#include "ui_databaseconnectionedititemwidget.h"

DatabaseConnectionEditItemWidget::DatabaseConnectionEditItemWidget(
        QStringList &connectionData, QWidget *parent) :
    QDialog(parent),
    refToConnectionData(connectionData),
    ui(new Ui::DatabaseConnectionEditItemWidget)
{
    ui->setupUi(this);

    if(!refToConnectionData.empty())
    {
        ui->lineEdit_ConnectionName->setText(refToConnectionData[0]);
        ui->lineEdit_HostName->setText(refToConnectionData[1]);
        ui->lineEdit_DatabaseName->setText(refToConnectionData[2]);
        ui->lineEdit_UserName->setText(refToConnectionData[3]);
        ui->lineEdit_Password->setText(refToConnectionData[4]);
    }
}

DatabaseConnectionEditItemWidget::~DatabaseConnectionEditItemWidget()
{
    delete ui;
}

void DatabaseConnectionEditItemWidget::on_pushButton_Done_clicked()
{
    if(!ui->lineEdit_ConnectionName->text().isEmpty() &&
            !ui->lineEdit_HostName->text().isEmpty() &&
            !ui->lineEdit_DatabaseName->text().isEmpty() &&
            !ui->lineEdit_UserName->text().isEmpty() &&
            !ui->lineEdit_Password->text().isEmpty())
    {
        refToConnectionData.clear();
        refToConnectionData += ui->lineEdit_ConnectionName->text();
        refToConnectionData += ui->lineEdit_HostName->text();
        refToConnectionData += ui->lineEdit_DatabaseName->text();
        refToConnectionData += ui->lineEdit_UserName->text();
        refToConnectionData += ui->lineEdit_Password->text();
        this->accept();
    }
}

void DatabaseConnectionEditItemWidget::on_pushButton_Exit_clicked()
{
    this->reject();
}
