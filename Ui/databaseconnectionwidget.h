#ifndef DATABASECONNECTIONWIDGET_H
#define DATABASECONNECTIONWIDGET_H

#include <QDialog>
#include "DataBase/databasemanager.h"

#include "Ui/databaseconnectioneditwidget.h"

namespace Ui {
class DatabaseConnectionWidget;
}

/// Represents the user-interface for database connection \n
/// <tt> +------------+------------+ </tt>\n
/// <tt> |Connections |  ComboBox  | </tt>\n
/// <tt> +------------+------------+ </tt>\n
/// <tt> |HostName    |............| </tt>\n
/// <tt> |DatabaseName|............| </tt>\n
/// <tt> |UserName    |............| </tt>\n
/// <tt> +--------+---+---+--------+ </tt>\n
/// <tt> |Connect | Edit  | Cancel | </tt>\n
/// <tt> +--------+-------+--------+ </tt>\n
///
/// Emits accept() on connection and reject() otherwise
class DatabaseConnectionWidget : public QDialog
{
    Q_OBJECT

        /// Stores connection data,
        /// data can be selected by ComboBox
    private: QList<QStringList> _myConnectionsData;

    private: void _loadConnections();
    private: void _fillConnectionsList();

    public : explicit DatabaseConnectionWidget(QWidget *parent = 0);
    public : ~DatabaseConnectionWidget();

private Q_SLOTS:
    void on_pushButton_Connect_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Edit_clicked();

    void on_comboBox_Connections_currentIndexChanged(int index);

private: Ui::DatabaseConnectionWidget *ui;
};

#endif // DATABASECONNECTIONWIDGET_H
