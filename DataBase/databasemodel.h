#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QDateTime>
#include <QPolygonF>
#include <QStandardItem>
#include <QStandardItemModel>
#include <opencv2/opencv.hpp>

// \todo find out that you really need to duplicate those pointers with vectors

#define SURVEY_TYPE     1001
#define WOUND_TYPE      1002
#define PATIENT_TYPE    1003
#define DOCTOR_TYPE     1004

class Survey : public QStandardItem
{
    public : Survey(const QIcon &icon, const QDateTime &date, int ID, const QString &notes):
        QStandardItem(icon, date.toString("dd.MM.yyyy hh:mm")),
        id(ID),
        date(date),
        notes(notes){}
    public : int id;
    public : QDateTime date;
    public : QString notes;
    public : cv::Mat image;

    public : QVector<QPolygonF> polygons;
    public : QPolygonF rulerPoints;
    public : double rulerFactor;
    public : double woundArea;

    public : int type() const override { return SURVEY_TYPE;}
};

class Wound : public QStandardItem
{
    public : Wound(const QIcon &icon, const QString &text, int ID, const QString &notes):
        QStandardItem(icon, text),
        id(ID),
        name(text),
        notes(notes){}
    public : int id;
    public : QString name;
    public : QString notes;
    public : QVector<Survey*> surveys;
    public : void addSurvey(Survey* newSurvey)
    {
        this->appendRow(newSurvey);
        this->surveys.append(newSurvey);
    }
    public : int type() const override { return WOUND_TYPE;}
};

class Patient : public QStandardItem
{
    public : Patient(const QIcon &icon, const QString &text, int ID, const QString &notes):
        QStandardItem(icon, text),
        id(ID),
        name(text),
        notes(notes){}
    public : int id;
    public : QString name;
    public : QString notes;
    public : QVector<Wound*> wounds;
    public : void addWound(Wound* newWound)
    {
        this->appendRow(newWound);
        this->wounds.append(newWound);
    }
    public : int type() const override { return PATIENT_TYPE;}
};

class Doctor : public QStandardItem
{
    public : Doctor(const QIcon &icon, const QString &text, int ID, const QString &notes):
        QStandardItem(icon, text),
        id(ID),
        name(text),
        notes(notes){}
    public : int id;
    public : QString name;
    public : QString notes;
    public : QVector<Patient*> patients;
    public : void addPatient(Patient* newPatient)
    {
        this->appendRow(newPatient);
        this->patients.append(newPatient);
    }
    public : int type() const override { return DOCTOR_TYPE;}
};

class DatabaseModel
{
    public : QStandardItemModel * model = nullptr;
    public : Doctor* doctor = nullptr;

    public : DatabaseModel(QObject * parent = 0): model(new QStandardItemModel(parent)){}
    public : void addDoctor(Doctor* newDoctor)
    {
        model->invisibleRootItem()->appendRow(newDoctor);
        doctor = newDoctor;
    }
    public : ~DatabaseModel(){}
};

#endif // DATABASEMODEL_H
