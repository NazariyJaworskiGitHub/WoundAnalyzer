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
    Log::StaticLogger::instance() << "[Database] connecting to " +
                                     dbName.toStdString() + " at " +
                                     hName.toStdString() + " with login " +
                                     uName.toStdString() + "\n";

    QSqlDatabase _db = QSqlDatabase::addDatabase(
                "QMYSQL",DATABASENAME);
//    _db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=WoundData.mdb");
    _db.setHostName(hName);
    _db.setDatabaseName(dbName);
    _db.setUserName(uName);
    _db.setPassword(password);
    _db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    _isConnectedFlag = _db.open();

    if(_isConnectedFlag)
        Log::StaticLogger::instance() << "[Database] connection succeed\n";
    else
        Log::StaticLogger::instance() << "[Database] <FAIL> "
                                         + lastError().text().toStdString() + "\n";

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

QStandardItemModel *DatabaseManager::prepareDatabaseModel(QObject *paretn)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    QSqlRecord record;
    QString str;
    int doctorId;

    QStandardItemModel * model = new QStandardItemModel(paretn);
    QStandardItem *currentItem = model->invisibleRootItem();

    /////////////////////////////////////////////////////////////////////////////////////
    {
        Log::StaticLogger::instance() << "[Database] searching id of " +
                QSqlDatabase::database(DATABASENAME).userName().toStdString() + "\n";
        str = "SELECT ID FROM Doctors WHERE DoctorName = '" +
                QSqlDatabase::database(DATABASENAME).userName() + "'";
        if(query.exec(str))
            Log::StaticLogger::instance() << "[Database] id found\n";
        else
        {
            Log::StaticLogger::instance() << "[Database] <FAIL> "
                                             + query.lastError().text().toStdString() + "\n";
            /// \todo error
        }
        record = query.record();
        query.next();
        doctorId = query.value(record.indexOf("ID")).toInt();
        QIcon doctorIcon(QStringLiteral("Icons/Doctor.png"));
        QStandardItem *item = new QStandardItem(
                    doctorIcon,
                    QSqlDatabase::database(DATABASENAME).userName());
        currentItem->appendRow(item);
        currentItem = item;
    }
    //////////////////////////////////////////////////////////////////////////////////////
    {
        Log::StaticLogger::instance() << "[Database] loading patients of " +
                QSqlDatabase::database(DATABASENAME).userName().toStdString() + "\n";
        str = "SELECT PatientName FROM Patients WHERE DoctorID = " + QString::number(doctorId);

        if(query.exec(str))
            Log::StaticLogger::instance() << "[Database] patients are loaded\n";
        else
        {
            Log::StaticLogger::instance() << "[Database] <FAIL> "
                                             + query.lastError().text().toStdString() + "\n";
            /// \todo error
        }
        QIcon patientIcon(QStringLiteral("Icons/Patient.png"));
        record = query.record();
        while(query.next())
        {
            Patient * patient = new Patient{query.value(record.indexOf("PatientName")).toString()};
            QStandardItem *item = new QStandardItem(patientIcon, patient->PatientName);
            currentItem->appendRow(item);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    return model;
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
