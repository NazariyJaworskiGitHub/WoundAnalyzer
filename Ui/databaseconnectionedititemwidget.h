#ifndef DATABASECONNECTIONEDITITEMWIDGET_H
#define DATABASECONNECTIONEDITITEMWIDGET_H

#include <QDialog>

namespace Ui {
class DatabaseConnectionEditItemWidget;
}

class DatabaseConnectionEditItemWidget : public QDialog
{
    Q_OBJECT

    public: QStringList &refToConnectionData;

        /// New connection item data will be stored at connectionData,
        /// which is the reference to external object
    public : explicit DatabaseConnectionEditItemWidget(
            QStringList &connectionData, QWidget *parent = 0);
    public : ~DatabaseConnectionEditItemWidget();

    private:Ui::DatabaseConnectionEditItemWidget *ui;

    private Q_SLOTS:
        /// If there is any user defined data, updates _myConnectionData,
        /// emits accept() and closes the dialog
        void on_pushButton_Done_clicked();
        void on_pushButton_Exit_clicked();


};

#endif // DATABASECONNECTIONEDITITEMWIDGET_H
