#include "databaseconnectionwidget.h"
#include "ui_databaseconnectionwidget.h"

#include <QMessageBox>

void DatabaseConnectionWidget::_fillConnectionsList()
{
    _myConnectionsData.clear();
    ui->comboBox_Connections->clear();
    _myConnectionsData = DatabaseManager::instance()->loadConnectionsFromFile();
    if(!_myConnectionsData.empty())
    {
        for(auto _connection : _myConnectionsData)
            ui->comboBox_Connections->addItem(_connection[0]);
        ui->pushButton_Connect->setEnabled(true);
        _myIndexOfSelectedConnection = 0;   //First connection
        ui->lineEdit_HostName->setText(_myConnectionsData[0][1]);
        ui->lineEdit_DatabaseName->setText(_myConnectionsData[0][2]);
        ui->lineEdit_UserName->setText(_myConnectionsData[0][3]);
    }
    else
    {
        ui->pushButton_Connect->setDisabled(true);
        _myIndexOfSelectedConnection = -1;
        ui->comboBox_Connections->addItem("No known connections");
        ui->lineEdit_HostName->setText("");
        ui->lineEdit_DatabaseName->setText("");
        ui->lineEdit_UserName->setText("");
    }
}

DatabaseConnectionWidget::DatabaseConnectionWidget(QWidget *parent) :
    QDialog(parent),
    _myIndexOfSelectedConnection(-1),
    ui(new Ui::DatabaseConnectionWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Connect to database");
    _fillConnectionsList();
}

DatabaseConnectionWidget::~DatabaseConnectionWidget()
{
    delete ui;
}

void DatabaseConnectionWidget::on_pushButton_Connect_clicked()
{
    if(_myIndexOfSelectedConnection!=-1)
    // i.e. if there is a connection item. Not necessary to check, becouse,
    // _myConnectButton will be disabled at _fillConnectionsList()
    {
        if(!DatabaseManager::instance()->connectToDatabase(
                    _myConnectionsData[_myIndexOfSelectedConnection][1],
                    _myConnectionsData[_myIndexOfSelectedConnection][2],
                    _myConnectionsData[_myIndexOfSelectedConnection][3],
                    //username is the password
                    _myConnectionsData[_myIndexOfSelectedConnection][3]))
        {
            QMessageBox msgBox;
            msgBox.setText("Connection failed");
            msgBox.setInformativeText(DatabaseManager::instance()->lastError().text());
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        else this->accept();    // close dialog
    }
    else
    {
        /// \todo
    }
}

void DatabaseConnectionWidget::on_pushButton_Cancel_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Connection canceled");
    msgBox.setInformativeText("You have to connect to some database");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void DatabaseConnectionWidget::on_pushButton_Edit_clicked()
{
//    this->hide();
//    DBCEditWidget _dbcew(true, nullptr, _myInternalStorage);
//    _dbcew.exec();
//    _fillConnectionsList(true, _myInternalStorage);
//    this->show();
    /// \todo
}
