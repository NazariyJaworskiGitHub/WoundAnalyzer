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

QStandardItemModel *DatabaseManager::prepareDatabaseModel(QObject *parent)
{
    QSqlRecord record;
    QString str;

    QIcon patientIcon(QStringLiteral("Icons/Patient.png"));
    QIcon doctorIcon(QStringLiteral("Icons/Doctor.png"));
    QIcon woundIcon(QStringLiteral("Icons/Wound.png"));
    QIcon surveyIcon(QStringLiteral("Icons/Survey.png"));

    if(DBrep) delete DBrep;
    DBrep = new DatabaseModel(parent);

    /////////////////////////////////////////////////////////////////////////////////////
    {
        QSqlQuery doctorsQuery(QSqlDatabase::database(DATABASENAME));
        Log::StaticLogger::instance() << "[Database] searching id of " + QSqlDatabase::database(DATABASENAME).userName().toStdString() + "\n";
        str = "SELECT * FROM Doctors WHERE DoctorName = '" + QSqlDatabase::database(DATABASENAME).userName() + "'";
        if(doctorsQuery.exec(str))
            Log::StaticLogger::instance() << "[Database] id found\n";
        else
        {
            Log::StaticLogger::instance() << "[Database] <FAIL> " + doctorsQuery.lastError().text().toStdString() + "\n";
            return nullptr;
        }
        doctorsQuery.next();
        record = doctorsQuery.record();
        DBrep->addDoctor(new Doctor(
                             doctorIcon,
                             QSqlDatabase::database(DATABASENAME).userName(),
                             doctorsQuery.value(record.indexOf("ID")).toInt()));
    }
    //////////////////////////////////////////////////////////////////////////////////////
    {
        QSqlQuery patientsQuery(QSqlDatabase::database(DATABASENAME));
        Log::StaticLogger::instance() << "[Database] loading patients of " + QSqlDatabase::database(DATABASENAME).userName().toStdString() + "\n";
        str = "SELECT * FROM Patients WHERE DoctorID = " + QString::number(DBrep->doctor->DoctorID);
        if(patientsQuery.exec(str))
            Log::StaticLogger::instance() << "[Database] patients are loaded\n";
        else
        {
            Log::StaticLogger::instance() << "[Database] <FAIL> " + patientsQuery.lastError().text().toStdString() + "\n";
            return nullptr;
        }
        while(patientsQuery.next())
        {
            record = patientsQuery.record();
            Patient * patient = new Patient(
                        patientIcon,
                        patientsQuery.value(record.indexOf("PatientName")).toString(),
                        patientsQuery.value(record.indexOf("ID")).toInt());
            DBrep->doctor->addPatient(patient);

            ///////////////////////////////////////////////////////////////////////////////
            {
                QSqlQuery woundsQuery(QSqlDatabase::database(DATABASENAME));
                Log::StaticLogger::instance() << "[Database] loading wounds of " + patient->PatientName.toStdString() + "\n";
                str = "SELECT * FROM Wounds WHERE PatientID = " + QString::number(patient->PatientID);
                if(woundsQuery.exec(str))
                    Log::StaticLogger::instance() << "[Database] wounds are loaded\n";
                else
                {
                    Log::StaticLogger::instance() << "[Database] <FAIL> " + woundsQuery.lastError().text().toStdString() + "\n";
                    return nullptr;
                }
                while(woundsQuery.next())
                {
                    record = woundsQuery.record();
                    Wound * wound = new Wound(
                                woundIcon,
                                woundsQuery.value(record.indexOf("WoundName")).toString(),
                                woundsQuery.value(record.indexOf("ID")).toInt(),
                                woundsQuery.value(record.indexOf("Notes")).toString());
                    patient->addWound(wound);
                    ///////////////////////////////////////////////////////////////////////
                    {
                        QSqlQuery surveyQuery(QSqlDatabase::database(DATABASENAME));
                        Log::StaticLogger::instance() << "[Database] loading surveys of " + wound->WoundName.toStdString() + "\n";
                        str = "SELECT * FROM Surveys WHERE WoundID = " + QString::number(wound->WoundID);
                        if(surveyQuery.exec(str))
                            Log::StaticLogger::instance() << "[Database] surveys are loaded\n";
                        else
                        {
                            Log::StaticLogger::instance() << "[Database] <FAIL> " + surveyQuery.lastError().text().toStdString() + "\n";
                            return nullptr;
                        }
                        while(surveyQuery.next())
                        {
                            record = surveyQuery.record();
                            Survey * survey = new Survey(
                                        surveyIcon,
                                        surveyQuery.value(record.indexOf("SurveyDate")).toDateTime(),
                                        surveyQuery.value(record.indexOf("ID")).toInt(),
                                        surveyQuery.value(record.indexOf("Notes")).toString());
                            wound->addSurvey(survey);
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////
                }
            }
            ///////////////////////////////////////////////////////////////////////////////
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    return DBrep->model;
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
    if(DBrep)
        delete DBrep;
}

DatabaseManager *DatabaseManager::instance()
{
    static DatabaseManager _instanceOfDatabaseManager;
    return &_instanceOfDatabaseManager;
}
