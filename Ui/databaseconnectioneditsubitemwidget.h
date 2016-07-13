#ifndef DATABASECONNECTIONEDITSUBITEMWIDGET_H
#define DATABASECONNECTIONEDITSUBITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "Ui/databaseconnectionedititemwidget.h"

/// Inline-widget for connection data representation, can't be used
/// without DatabaseConnectionEditItemWidget
class DatabaseConnectionEditSubItemWidget : public QWidget
{
    Q_OBJECT

    public : QStringList myConnectionData;
    private: QLabel      *_myConnectionDataLabel = nullptr;
    private: QPushButton *_myEditButton          = nullptr;
    private: QPushButton *_myDeleteButton        = nullptr;

    public : explicit DatabaseConnectionEditSubItemWidget(
            QStringList connectionData, QWidget *parent = 0):
        QWidget(parent),
        myConnectionData(connectionData)
    {
        _myConnectionDataLabel = new QLabel(this);
        _myEditButton = new QPushButton("Edit",this);
        _myDeleteButton = new QPushButton("Delete",this);
        connect(_myEditButton,SIGNAL(clicked()),SLOT(on_pushButton_Edit_clicked()));
        connect(_myDeleteButton,SIGNAL(clicked()),SLOT(on_pushButton_Delete_clicked()));

        QHBoxLayout *_layout = new QHBoxLayout();
        _layout->addWidget(_myConnectionDataLabel);
        _layout->addWidget(_myEditButton);
        _layout->addWidget(_myDeleteButton);
        this->setLayout(_layout);

        _update();
    }

    public : ~DatabaseConnectionEditSubItemWidget()
    {
    }

        /// Updates the _myConnectionDataLabel with myConnectionData
    private: void _update()
    {
        QString _str("<b>"+ myConnectionData[0] + "</b>" + ' ' +
            myConnectionData[1] + ' ' +
            myConnectionData[2] + ' ' +
            myConnectionData[3]);
        _myConnectionDataLabel->setText(_str);
    }

    private Q_SLOTS:
        /// Calls DatabaseConnectionEditItemWidget
    void on_pushButton_Edit_clicked()
    {
        DatabaseConnectionEditItemWidget _editItemWidget(myConnectionData);
        _editItemWidget.exec();
        _update();
    }
        /// Emits sDelete
    void on_pushButton_Delete_clicked()
    {
        Q_EMIT sDelete(this);
    }

        /// Catch this with parent object to destroy current object
    public : Q_SIGNAL void sDelete(DatabaseConnectionEditSubItemWidget* item);
};

#endif // DATABASECONNECTIONEDITSUBITEMWIDGET_H
