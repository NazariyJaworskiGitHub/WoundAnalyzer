#ifndef DATABASECONNECTIONWIDGET_H
#define DATABASECONNECTIONWIDGET_H

#include <QDialog>
#include "DataBase/databasemanager.h"

namespace Ui {
class DatabaseConnectionWidget;
}

class DatabaseConnectionWidget : public QDialog
{
    Q_OBJECT

        /// Stores connection data,
        /// data can be selected by ComboBox
    private: QList<QStringList> _myConnectionsData;

        /// Use to store index of selected connection from myConnectionsData,
        /// or \c -1 if last is empty
    private: int _myIndexOfSelectedConnection;

        /// Fills ComboBox by saved data:
        /// 1) clear the ComboBox, clear the _myConnectionsData;
        /// 2) try to read data from file;
        /// 3) if not empty, fill _myConnectionsData, fill the
        /// ComboBox, make the Connect button enabled,
        /// set the _myIndexOfSelectedConnection = 0;
        /// 4) elese, make the Connect button disabled,
        /// set the _myIndexOfSelectedConnection = -1, add "No known connections"
        /// item to the ComboBox;
    private: void _fillConnectionsList();

    public : explicit DatabaseConnectionWidget(QWidget *parent = 0);
    public : ~DatabaseConnectionWidget();

private Q_SLOTS:
    void on_pushButton_Connect_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Edit_clicked();

private: Ui::DatabaseConnectionWidget *ui;
};

#endif // DATABASECONNECTIONWIDGET_H
