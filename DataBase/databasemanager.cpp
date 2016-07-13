#include "databasemanager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

#define DATABASENAME "myDatabase"
DatabaseManager::DatabaseManager(QObject *parent):
    QObject(parent),
    _isConnectedFlag(false)
{
}

bool DatabaseManager::connectToDatabase(
        QString hName, QString dbName, QString uName, QString password)
{
    QSqlDatabase _db = QSqlDatabase::addDatabase(
                "QMYSQL",DATABASENAME);
    _db.setHostName(hName);
    _db.setDatabaseName(dbName);
    _db.setUserName(uName);
    _db.setPassword(password);
    _db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    _isConnectedFlag = _db.open();
    return _isConnectedFlag;
}

void DatabaseManager::saveConnectionsToFile(const QList<QStringList> &refToConnections,
                                            QString dbConnectionsFilename) const
{
    QFile _dbConnectionsFile(dbConnectionsFilename);
    _dbConnectionsFile.open(QIODevice::Text | QIODevice::WriteOnly);

    QTextStream _textStream(&_dbConnectionsFile);

    for(auto i: refToConnections)
    {
        for(auto j: i)
            _textStream << j << ' ';
        _textStream << '\n';
    }
    _dbConnectionsFile.close();
}

QList<QStringList> DatabaseManager::loadConnectionsFromFile(QString dbConnectionsFilename) const
{
    QList<QStringList> _connections;
    try
    {
        QFile _dbConnectionsFile(dbConnectionsFilename);
        _dbConnectionsFile.open(QIODevice::Text | QIODevice::ReadOnly);

        QTextStream _textStream(&_dbConnectionsFile);
        while (!_textStream.atEnd())
        {
            QString _qstrData = _textStream.readLine();

            for(int k=0;;k+=5)  //ConnectionName HostName DatabaseName UserName Password
            {
                // ConnectionName
                QStringList _connection(_qstrData.section(' ',k,k,QString::SectionSkipEmpty));
                if(!_connection[0].size())
                    break;
                else
                {
                    // HostName
                    _connection += _qstrData.section(' ',k+1,k+1,QString::SectionSkipEmpty);
                    // DatabaseName
                    _connection += _qstrData.section(' ',k+2,k+2,QString::SectionSkipEmpty);
                    // UserName
                    _connection += _qstrData.section(' ',k+3,k+3,QString::SectionSkipEmpty);
                    // Password
                    _connection += _qstrData.section(' ',k+4,k+4,QString::SectionSkipEmpty);
                    _connections.push_back(_connection);
                }
            }
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
    return QSqlDatabase::database(DATABASENAME, false).lastError();
}

DatabaseManager::~DatabaseManager()
{
    if(_isConnectedFlag)
    {
        {   //barcet statement for local variable _db
            QSqlDatabase _db = QSqlDatabase::database(DATABASENAME, false);
            _db.close();
        }
        QSqlDatabase::removeDatabase(DATABASENAME);
    }
}

DatabaseManager *DatabaseManager::instance()
{
    static DatabaseManager _instanceOfDatabaseManager;
    return &_instanceOfDatabaseManager;
}
