#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QAbstractItemModel>
#include <QDateTime>
#include <QPolygonF>
#include <opencv2/opencv.hpp>

//class DatabaseModel : public QAbstractItemModel
//{
//    Q_OBJECT

//    public: QVector<Doctor*> doctors;

//public:
//    DatabaseModel();
//    ~DatabaseModel();
//};

class Survey
{
    public : QDateTime SurveyDate;
    public : QString Notes;
    public : cv::Mat Image;

    public : cv::Mat ProcessedWoundImage;
    public : QVector<QPolygonF> Polygons;
    public : QPolygonF RulerPoints;
    public : double RulerFactor;
    public : double WoundArea;
};

class Wound
{
    public : QString Notes;
    public : QVector<Survey*> surveys;
};

class Patient
{
    public : QString PatientName;
    public : QVector<Wound*> wounds;
};

class Doctor
{
    public : QString DoctorName;
    public : QVector<Patient*> patients;
};

#endif // DATABASEMODEL_H
