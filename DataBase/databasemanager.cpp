#include "databasemanager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

DatabaseManager::DatabaseManager(QObject *parent):
    QObject(parent),
    _isConnectedFlag(false)
{
}

bool DatabaseManager::connectToDatabase(
        QString hName, QString dbName, QString uName, QString password)
{
    QSqlDatabase _db = QSqlDatabase::addDatabase(
                "QMYSQL",QLatin1String(QSqlDatabase::defaultConnection));
    _db.setHostName(hName);
    _db.setDatabaseName(dbName);
    _db.setUserName(uName);
    _db.setPassword(password);
    _db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    _isConnectedFlag = _db.open();
    return _isConnectedFlag;
}

void DatabaseManager::saveConnectionsToFile(QString dbConnectionsFilename)
{
//    QFile _dbConnectionsFile(dbConnectionsFilename);
//    _dbConnectionsFile.open(QIODevice::Text | QIODevice::WriteOnly);
//    ///TODO
//    _dbConnectionsFile.flush();
//    _dbConnectionsFile.close();
}

QList<QStringList> DatabaseManager::loadConnectionsFromFile(QString dbConnectionsFilename)
{
    QList<QStringList> _connections;
    try
    {
        QFile _dbConnectionsFile(dbConnectionsFilename);
        _dbConnectionsFile.open(QIODevice::Text | QIODevice::ReadOnly);

        QTextStream _textStream(&_dbConnectionsFile);
        QStringList _qstrData;
        //ConnectionName HostName DatabaseName UserName Password
        while (!_textStream.atEnd())
        {
            _qstrData << _textStream.readLine();
            _connections.push_back(_qstrData);
        }
        _dbConnectionsFile.close();
    }
    catch(std::exception &e)
    {
        // some error occurred
        // return empty list
    }
    return _connections;
}

QSqlError DatabaseManager::lastError() const
{
    return QSqlDatabase::database(
                QLatin1String(QSqlDatabase::defaultConnection), false).lastError();
}

DatabaseManager::~DatabaseManager()
{
    if(_isConnectedFlag)
    {
        {   //barcet statement for local variable _db
            QSqlDatabase _db = QSqlDatabase::database(
                        QLatin1String(QSqlDatabase::defaultConnection), false);
            _db.close();
        }
        QSqlDatabase::removeDatabase(QLatin1String(QSqlDatabase::defaultConnection));
    }
}

DatabaseManager *DatabaseManager::instance()
{
    static DatabaseManager _instanceOfDatabaseManager;
    return &_instanceOfDatabaseManager;
}
