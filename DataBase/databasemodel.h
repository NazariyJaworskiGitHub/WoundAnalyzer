#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QDateTime>
#include <QPolygonF>
#include <QStandardItem>
#include <QStandardItemModel>
#include <opencv2/opencv.hpp>

// \todo find out that you really need to duplicate those pointers with vectors

class Survey : public QStandardItem
{
    public : Survey(const QIcon &icon, const QDateTime &date, int ID, const QString &notes):
        QStandardItem(icon, date.toString()),
        SurveyID(ID),
        SurveyDate(date),
        Notes(notes){}

    public : int SurveyID;
    public : QDateTime SurveyDate;
    public : QString Notes;
    public : cv::Mat Image;

    public : cv::Mat ProcessedWoundImage;
    public : QVector<QPolygonF> Polygons;
    public : QPolygonF RulerPoints;
    public : double RulerFactor;
    public : double WoundArea;
};

class Wound : public QStandardItem
{
    public : Wound(const QIcon &icon, const QString &text, int ID, const QString &notes):
        QStandardItem(icon, text),
        WoundID(ID),
        WoundName(text),
        Notes(notes){}
    public : int WoundID;
    public : QString WoundName;
    public : QString Notes;
    public : QVector<Survey*> surveys;
    public : void addSurvey(Survey* newSurvey)
    {
        this->appendRow(newSurvey);
        this->surveys.append(newSurvey);
    }
};

class Patient : public QStandardItem
{
    public : Patient(const QIcon &icon, const QString &text, int ID):
        QStandardItem(icon, text),
        PatientID(ID),
        PatientName(text){}
    public : int PatientID;
    public : QString PatientName;
    public : QVector<Wound*> wounds;
    public : void addWound(Wound* newWound)
    {
        this->appendRow(newWound);
        this->wounds.append(newWound);
    }
};

class Doctor : public QStandardItem
{
    public : Doctor(const QIcon &icon, const QString &text, int ID):
        QStandardItem(icon, text),
        DoctorID(ID),
        DoctorName(text){}
    public : int DoctorID;
    public : QString DoctorName;
    public : QVector<Patient*> patients;
    public : void addPatient(Patient* newPatient)
    {
        this->appendRow(newPatient);
        this->patients.append(newPatient);
    }
};

class DatabaseModel
{
    public : QStandardItemModel * model = nullptr;
    public : Doctor* doctor = nullptr;

    public : DatabaseModel(QObject * parent = 0): model(new QStandardItemModel(parent)){}
//    public : void clear()
//    {
//        for(Patient* p: doctor->patients)
//        {
//            for(Wound* w: p->wounds)
//            {
//                for(Survey* s: w->surveys)
//                {
//                    s->Polygons.clear();
//                    delete s;
//                }
//                delete w;
//            }
//            delete p;
//        }
//        delete doctor;
//        doctor = nullptr;
//    }
    public : void addDoctor(Doctor* newDoctor)
    {
        model->invisibleRootItem()->appendRow(newDoctor);
        doctor = newDoctor;
    }
    public : ~DatabaseModel()
    {
//        clear();
    }
};

#endif // DATABASEMODEL_H
