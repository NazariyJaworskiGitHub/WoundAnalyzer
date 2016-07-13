#ifndef DATABASECONNECTIONEDITWIDGET_H
#define DATABASECONNECTIONEDITWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "Ui/databaseconnectioneditsubitemwidget.h"
#include "Ui/databaseconnectionedititemwidget.h"

/// Represents the user-interface for database connection \n
/// <tt> +---------------------------------------------------------+ </tt>\n
/// <tt> |Avaliable connections:                                   | </tt>\n
/// <tt> +---------------------------------------------------------+ </tt>\n
/// <tt> |ConnectionName HostName DatabaseName UserName|Edit|Delete| </tt>\n
/// <tt> |   ............                                          | </tt>\n
/// <tt> +----------------------------+----------------------------+ </tt>\n
/// <tt> |          Create            |            Exit            | </tt>\n
/// <tt> +----------------------------+----------------------------+ </tt>\n

class DatabaseConnectionEditWidget : public QDialog
{
    Q_OBJECT

    private: QLabel  *_mainText;
    private: QVBoxLayout *_mainLayout;
    private: QPushButton *_create;
    private: QPushButton *_exit;

    private: QList<DatabaseConnectionEditSubItemWidget*> _myItems;
    private: QList<QStringList> &_refToConnectionsData;

    private: void _addItem(QStringList &item)
    {
        _myItems.push_back(new DatabaseConnectionEditSubItemWidget(item, this));
        connect(_myItems.last(), SIGNAL(sDelete(DatabaseConnectionEditSubItemWidget*)),
                this, SLOT(on_ChildItemDelete(DatabaseConnectionEditSubItemWidget *)));
        _mainLayout->insertWidget(_mainLayout->count() - 2, _myItems.last());
        if(_myItems.size() > 0)
                _mainText->setText("Available connections:");
    }

    public : explicit DatabaseConnectionEditWidget(
            QList<QStringList> &connectionsData, QWidget *parent = 0) :
        QDialog(parent),
        _refToConnectionsData(connectionsData)
    {
        _mainText = new QLabel(this);
        _create = new QPushButton("Create", this);
        _exit = new QPushButton("Exit", this);
        QHBoxLayout *_buttonsLayout = new QHBoxLayout();
        _buttonsLayout->addWidget(_create);
        _buttonsLayout->addStretch();
        _buttonsLayout->addWidget(_exit);
        _mainLayout = new QVBoxLayout();
        _mainLayout->addWidget(_mainText);
        for(auto i:_refToConnectionsData)
        {
            _myItems.push_back(new DatabaseConnectionEditSubItemWidget(i, this));
            connect(_myItems.last(), SIGNAL(sDelete(DatabaseConnectionEditSubItemWidget*)),
                    this, SLOT(on_ChildItemDelete(DatabaseConnectionEditSubItemWidget *)));
            _mainLayout->addWidget(_myItems.last());
        }
        _mainLayout->addItem(_buttonsLayout);
        _mainLayout->addStretch();
        if(_myItems.size() != 0)
            _mainText->setText("Available connections:");
        else
            _mainText->setText("No available connections");
        this->setLayout(_mainLayout);
        connect(_create,SIGNAL(clicked()),this,SLOT(on_pushButton_Create_clicked()));
        connect(_exit,SIGNAL(clicked()),this,SLOT(on_pushButton_Exit_clicked()));
        connect(this,SIGNAL(accepted()),this,SLOT(_updateConnectionsData()));
        connect(this,SIGNAL(rejected()),this,SLOT(_updateConnectionsData()));
    }
    public : ~DatabaseConnectionEditWidget()
    {
    }

private Q_SLOTS:
    void _updateConnectionsData()
    {
        _refToConnectionsData.clear();
        for(auto i:_myItems)
            _refToConnectionsData.push_back(i->myConnectionData);
    }
    void on_pushButton_Create_clicked()
    {
        QStringList _newItem;   // it live here
        DatabaseConnectionEditItemWidget _editItemWidget(_newItem, this);
        _editItemWidget.exec();
        if(!_newItem.empty())
            _addItem(_newItem); // there will be a copy
    }

    void on_pushButton_Exit_clicked()
    {
        this->reject();
    }

    void on_ChildItemDelete(DatabaseConnectionEditSubItemWidget *caller)
    {
        for(QList<DatabaseConnectionEditSubItemWidget*>::Iterator i = _myItems.begin();
            i!=_myItems.end();i++)
        {
            if((*i) == caller)
            {
                _mainLayout->removeWidget(caller);
                delete caller;
                _myItems.erase(i);
                break;
            }
        }
        if(_myItems.size() == 0)
            _mainText->setText("No available connections");
    }
};

#endif // DATABASECONNECTIONEDITWIDGET_H
