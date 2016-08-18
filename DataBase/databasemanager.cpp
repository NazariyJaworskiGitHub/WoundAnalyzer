#include "databasemanager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <Image/imagemanager.h>

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

    if(DBrep) delete DBrep;
    DBrep = new DatabaseModel(parent);

    /////////////////////////////////////////////////////////////////////////////////////
    {
        QSqlQuery doctorsQuery(QSqlDatabase::database(DATABASENAME));
        Log::StaticLogger::instance() << "[Database] searching id of " + QSqlDatabase::database(DATABASENAME).userName().toStdString() + "\n";
        str = "SELECT ID, Notes FROM Doctors WHERE DoctorName = '" + QSqlDatabase::database(DATABASENAME).userName() + "'";
        if(doctorsQuery.exec(str))
            Log::StaticLogger::instance() << "[Database] id found\n";
        else
        {
            Log::StaticLogger::instance() << "[Database] <FAIL> " + doctorsQuery.lastError().text().toStdString() + "\n";
            return nullptr;
        }
        doctorsQuery.next();
        record = doctorsQuery.record();
        Doctor * newDoctor = new Doctor(
                    DBrep->doctorIcon,
                    QSqlDatabase::database(DATABASENAME).userName(),
                    doctorsQuery.value(record.indexOf("ID")).toInt(),
                    doctorsQuery.value(record.indexOf("Notes")).toString());
        DBrep->model->invisibleRootItem()->appendRow(newDoctor);
        DBrep->doctor = newDoctor;
    }
    //////////////////////////////////////////////////////////////////////////////////////
    {
        QSqlQuery patientsQuery(QSqlDatabase::database(DATABASENAME));
        Log::StaticLogger::instance() << "[Database] loading patients of " + QSqlDatabase::database(DATABASENAME).userName().toStdString() + "\n";
        str = "SELECT ID, PatientName, Notes FROM Patients WHERE DoctorID = " + QString::number(DBrep->doctor->id);
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
                        DBrep->patientIcon,
                        patientsQuery.value(record.indexOf("PatientName")).toString(),
                        patientsQuery.value(record.indexOf("ID")).toInt(),
                        patientsQuery.value(record.indexOf("Notes")).toString());
            DBrep->doctor->appendRow(patient);

            ///////////////////////////////////////////////////////////////////////////////
            {
                QSqlQuery woundsQuery(QSqlDatabase::database(DATABASENAME));
                Log::StaticLogger::instance() << "[Database] loading wounds of " + patient->name.toStdString() + "\n";
                str = "SELECT ID, WoundName, Notes FROM Wounds WHERE PatientID = " + QString::number(patient->id);
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
                                DBrep->woundIcon,
                                woundsQuery.value(record.indexOf("WoundName")).toString(),
                                woundsQuery.value(record.indexOf("ID")).toInt(),
                                woundsQuery.value(record.indexOf("Notes")).toString());
                    patient->appendRow(wound);
                    ///////////////////////////////////////////////////////////////////////
                    {
                        QSqlQuery surveyQuery(QSqlDatabase::database(DATABASENAME));
                        Log::StaticLogger::instance() << "[Database] loading surveys of " + wound->name.toStdString() + "\n";
                        str = "SELECT ID, SurveyDate, Notes, WoundArea FROM Surveys WHERE WoundID = " + QString::number(wound->id);
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
                                        DBrep->surveyIcon,
                                        surveyQuery.value(record.indexOf("SurveyDate")).toDateTime(),
                                        surveyQuery.value(record.indexOf("ID")).toInt(),
                                        surveyQuery.value(record.indexOf("Notes")).toString(),
                                        surveyQuery.value(record.indexOf("WoundArea")).toDouble());
                            wound->appendRow(survey);
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

bool DatabaseManager::loadSurveyWoundImage(Survey *target)
{
    QSqlRecord record;
    QString str;
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] loading survey wound image of " + target->date.toString("dd.MM.yyyy hh:mm").toStdString() + "\n";
    str = "SELECT Image, Polygons, RulerPoints, RulerFactor FROM Surveys WHERE ID = " + QString::number(target->id);
    if(query.exec(str))
        Log::StaticLogger::instance() << "[Database] survey wound image is loaded\n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return false;
    }
    query.next();
    record = query.record();
    QByteArray byteArray1 = query.value(record.indexOf("Image")).toByteArray();
    if(!byteArray1.isEmpty())
    {
        std::vector<char> v(byteArray1.data(), byteArray1.data() + byteArray1.length());
        target->image = cv::imdecode(v, cv::IMREAD_COLOR);
    }
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> survey wound image is empty\n";
        return false;
    }
    target->unpackPolygons(query.value(record.indexOf("Polygons")).toByteArray());
    target->unpackRulerPoints(query.value(record.indexOf("RulerPoints")).toByteArray());
    target->rulerFactor = query.value(record.indexOf("RulerFactor")).toDouble();
    return true;
}

bool DatabaseManager::_updateUtil(const QString &param, const QString &name, const QString &notes, const int id) const
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] updating " + param.toStdString() + " " + name.toStdString() + "\n";
    query.prepare("UPDATE "+ param + "s SET " + param + "Name = '" + name +
                  "', Notes = :notes WHERE ID = " + QString::number(id));
    query.bindValue(":notes", notes);
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] " + param.toStdString() + " is updated \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return false;
    }
    return true;
}

bool DatabaseManager::update(Patient *target)
{
    return _updateUtil("patient", target->name, target->notes, target->id);
}

bool DatabaseManager::update(Wound *target)
{
    return _updateUtil("wound", target->name, target->notes, target->id);
}

bool DatabaseManager::update(Doctor *target)
{
    return _updateUtil("doctor", target->name, target->notes, target->id);
}

bool DatabaseManager::update(Survey *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] updating survey " + target->date.toString("dd.MM.yyyy hh:mm").toStdString() + "\n";
    std::vector<unsigned char> v;
    cv::imencode(".jpg", target->image, v);
    query.prepare(
            "UPDATE Surveys SET "
            "SurveyDate = '" + target->date.toString("yyyy-MM-dd hh:mm:ss") + "', "
            "Notes = :notes, "
            "Image = :imageData, "
            "Polygons = :polygonsData, "
            "RulerPoints = :rulerPoints, "
            "RulerFactor = " + QString::number(target->rulerFactor) + ", "
            "WoundArea = " + QString::number(target->woundArea) + " "
            "WHERE ID = " + QString::number(target->id));
    query.bindValue(":notes", target->notes);
    query.bindValue(":imageData", QByteArray(reinterpret_cast<const char*>(v.data()),v.size()));
    query.bindValue(":polygonsData", target->packPolygons());
    query.bindValue(":rulerPoints", target->packRulerPoints());
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] survey is updated \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return false;
    }
    return true;
}

Patient * DatabaseManager::add(Doctor *parent)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] adding new patient to doctor " + parent->name.toStdString() + "\n";
    Patient *newTarget = new Patient(DBrep->patientIcon,"New patient", -1, "");
    query.prepare(
                "INSERT INTO Patients (DoctorID, PatientName)"
                "VALUES ('" + QString::number(parent->id) + "','New patient');");
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] new patient is added \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return nullptr;
    }
    newTarget->id = query.lastInsertId().toInt();
    parent->appendRow(newTarget);
    return newTarget;
}

Wound *DatabaseManager::add(Patient *parent)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] adding new wound to patient " + parent->name.toStdString() + "\n";
    Wound *newTarget = new Wound(DBrep->woundIcon,"New wound", -1, "");
    query.prepare(
                "INSERT INTO Wounds (PatientID, WoundName)"
                "VALUES ('" + QString::number(parent->id) + "','New wound');");
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] new wound is added \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return nullptr;
    }
    newTarget->id = query.lastInsertId().toInt();
    parent->appendRow(newTarget);
    return newTarget;
}

Survey *DatabaseManager::add(Wound *parent)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] adding new survey to wound " + parent->name.toStdString() + "\n";
    Survey *newTarget = new Survey(DBrep->surveyIcon, QDateTime::currentDateTime(), -1, "", 0);
    newTarget->image = ImageManager::instance()->getImage().clone();
    query.prepare(
                "INSERT INTO Surveys (WoundID, SurveyDate, Image)"
                "VALUES ('" + QString::number(parent->id) + "','" +
                newTarget->date.toString("yyyy-MM-dd hh:mm:ss") + "',:imageData);");
    std::vector<unsigned char> v;
    cv::imencode(".jpg", newTarget->image.clone(), v);
    query.bindValue(":imageData", QByteArray(reinterpret_cast<const char*>(v.data()),v.size()));
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] new survey is added \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return nullptr;
    }
    newTarget->id = query.lastInsertId().toInt();
    parent->appendRow(newTarget);
    return newTarget;
}

Wound *DatabaseManager::del(Survey *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] deleting survey " + target->date.toString("dd.MM.yyyy hh:mm").toStdString() + "\n";
    query.prepare( "DELETE FROM Surveys WHERE ID = " + QString::number(target->id));
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] survey is deleted \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return nullptr;
    }
    Wound *parent = static_cast<Wound*>(target->parent());
    parent->removeRow(target->row());
    return parent;
}

Patient *DatabaseManager::del(Wound *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] deleting wound " + target->name.toStdString() + "\n";
    query.prepare( "DELETE FROM Wounds WHERE ID = " + QString::number(target->id));
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] wound is deleted \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return nullptr;
    }
    Patient *parent = static_cast<Patient*>(target->parent());
    parent->removeRow(target->row());
    return parent;
}

Doctor *DatabaseManager::del(Patient *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::StaticLogger::instance() << "[Database] deleting patient " + target->name.toStdString() + "\n";
    query.prepare( "DELETE FROM Patients WHERE ID = " + QString::number(target->id));
    if(query.exec())
        Log::StaticLogger::instance() << "[Database] patient is deleted \n";
    else
    {
        Log::StaticLogger::instance() << "[Database] <FAIL> " + query.lastError().text().toStdString() + "\n";
        return nullptr;
    }
    Doctor *parent = static_cast<Doctor*>(target->parent());
    parent->removeRow(target->row());
    return parent;
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
