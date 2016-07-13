#include "databaseconnectionwidget.h"
#include "ui_databaseconnectionwidget.h"

#include <QMessageBox>

void DatabaseConnectionWidget::_loadConnections()
{
    _myConnectionsData.clear();
    _myConnectionsData = DatabaseManager::instance()->loadConnectionsFromFile();
    _fillConnectionsList();
}

void DatabaseConnectionWidget::_fillConnectionsList()
{
    ui->comboBox_Connections->blockSignals(true);
    ui->comboBox_Connections->clear();
    ui->comboBox_Connections->blockSignals(false);
    if(!_myConnectionsData.empty())
    {
        for(auto _connection : _myConnectionsData)
            ui->comboBox_Connections->addItem(_connection[0]);
        ui->pushButton_Connect->setEnabled(true);
    }
    else
    {
        ui->pushButton_Connect->setDisabled(true);
        ui->comboBox_Connections->addItem("No known connections");
        ui->lineEdit_HostName->setText("");
        ui->lineEdit_DatabaseName->setText("");
        ui->lineEdit_UserName->setText("");
    }
}

DatabaseConnectionWidget::DatabaseConnectionWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseConnectionWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Connect to database");
    _loadConnections();
}

DatabaseConnectionWidget::~DatabaseConnectionWidget()
{
    delete ui;
}

void DatabaseConnectionWidget::on_pushButton_Connect_clicked()
{
    if(!_myConnectionsData.empty())
    // i.e. if there is a connection item. Not necessary to check, becouse,
    // _myConnectButton will be disabled at _fillConnectionsList()
    {
        int _index = ui->comboBox_Connections->currentIndex();
        if(!DatabaseManager::instance()->connectToDatabase(
                    _myConnectionsData[_index][1],
                    _myConnectionsData[_index][2],
                    _myConnectionsData[_index][3],
                    _myConnectionsData[_index][4]))
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
        // it impossible to go here - button should be not active
    }
}

void DatabaseConnectionWidget::on_pushButton_Cancel_clicked()
{
//    QMessageBox msgBox;
//    msgBox.setText("Connection canceled");
//    msgBox.setInformativeText("You have to connect to some database");
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    msgBox.setIcon(QMessageBox::Information);
//    msgBox.exec();
    /// \todo exit application
    this->reject();
}

void DatabaseConnectionWidget::on_pushButton_Edit_clicked()
{
    DatabaseConnectionEditWidget _dbcew(_myConnectionsData, this);
    _dbcew.exec();
    _fillConnectionsList();
    DatabaseManager::instance()->saveConnectionsToFile(_myConnectionsData);
}

void DatabaseConnectionWidget::on_comboBox_Connections_currentIndexChanged(int index)
{
    if(!_myConnectionsData.empty())
    {
        ui->lineEdit_HostName->setText(_myConnectionsData[index][1]);
        ui->lineEdit_DatabaseName->setText(_myConnectionsData[index][2]);
        ui->lineEdit_UserName->setText(_myConnectionsData[index][3]);
    }
}
