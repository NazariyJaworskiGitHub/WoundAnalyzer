#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QDateTime>
#include <QPolygonF>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QIcon>
#include <opencv2/opencv.hpp>

// \todo find out that you really need to duplicate those pointers with vectors

#define SURVEY_TYPE     1001
#define WOUND_TYPE      1002
#define PATIENT_TYPE    1003
#define DOCTOR_TYPE     1004

class Survey : public QStandardItem
{
    public : Survey(const QIcon &icon, const QDateTime &date, int ID, const QString &notes, const double woundArea):
        QStandardItem(icon, date.toString("dd.MM.yyyy hh:mm") + " " + ((woundArea != 0) ? (QString::number(woundArea) + "sm2") : (""))),
        id(ID),
        date(date),
        notes(notes),
        woundArea(woundArea){}
    public : int id;
    public : QDateTime date;
    public : QString notes;
    public : cv::Mat image;

    public : QVector<QPolygonF> polygons;
    public : QByteArray packPolygons() const
    {
        QByteArray arr;
        char intSize = sizeof(int);
        arr.append(&intSize, 1);                            // size of int
        char qrealSize = sizeof(qreal);
        arr.append(&qrealSize, 1);                          // size of double
        int count = polygons.size();
        arr.append((char*)&count, sizeof(int));             // number of polygons <int>
        for(QPolygonF p: polygons)
        {
            int countP = p.size();
            arr.append((char*)&countP, sizeof(int));        // number of nodes in polygons <int>
        }
        for(QPolygonF p: polygons)
            for(QPointF n: p)
            {
                qreal x = n.x();
                qreal y = n.y();
                arr.append((char*)&x, sizeof(qreal));       // nodes of polygons <double>
                arr.append((char*)&y, sizeof(qreal));
            }
        return arr;
    }
    public : void unpackPolygons(QByteArray buf)
    {
        for(QPolygonF *p = polygons.begin(); p!= polygons.end(); ++p)
            p->clear();
        polygons.clear();
        if(!buf.isEmpty())
        {
            char * ptr = buf.data();
            size_t intSize = *ptr;
            ptr += sizeof(char);
            size_t qrealSize = *ptr;
            ptr += sizeof(char);
            int polygonsCount = *(int*)(ptr);
            ptr += intSize;
            polygons.resize(polygonsCount);
            for(QPolygonF *p = polygons.begin(); p!= polygons.end(); ++p)
            {
                int nodesCount = *(int*)(ptr);
                ptr += intSize;
                p->resize(nodesCount);
            }
            for(QPolygonF *p = polygons.begin(); p!= polygons.end(); ++p)
                for(QPointF *n = p->begin(); n!= p->end(); ++n)
                {
                    n->setX(*(qreal*)(ptr));
                    ptr += qrealSize;
                    n->setY(*(qreal*)(ptr));
                    ptr += qrealSize;
                }
        }
    }
    public : QPolygonF rulerPoints;
    public : QByteArray packRulerPoints() const
    {
        QByteArray arr;
        char intSize = sizeof(int);
        arr.append(&intSize, 1);                            // size of int
        char qrealSize = sizeof(qreal);
        arr.append(&qrealSize, 1);                          // size of double
        int count = rulerPoints.size();
        arr.append((char*)&count, sizeof(int));             // number of nodes <int>
        for(QPointF n: rulerPoints)
        {
            qreal x = n.x();
            qreal y = n.y();
            arr.append((char*)&x, sizeof(qreal));           // nodes of polygon <double>
            arr.append((char*)&y, sizeof(qreal));
        }
        return arr;
    }
    public : void unpackRulerPoints(QByteArray buf)
    {
        rulerPoints.clear();
        if(!buf.isEmpty())
        {
            char * ptr = buf.data();
            size_t intSize = *ptr;
            ptr += sizeof(char);
            size_t qrealSize = *ptr;
            ptr += sizeof(char);
            int nodesCount = *(int*)(ptr);
            ptr += intSize;
            rulerPoints.resize(nodesCount);
            for(QPointF *n = rulerPoints.begin(); n!= rulerPoints.end(); ++n)
            {
                n->setX(*(qreal*)(ptr));
                ptr += qrealSize;
                n->setY(*(qreal*)(ptr));
                ptr += qrealSize;
            }
        }
    }
    public : void applyPolygonsAndRulerPoints(
            const QVector<QPolygonF> &p,
            const QPolygonF &r)
    {
        for(QPolygonF *_p = polygons.begin(); _p!= polygons.end(); ++_p)
            _p->clear();
        polygons.clear();
        rulerPoints.clear();

        polygons.resize(p.size());
        for(int i=0 ; i < polygons.size(); ++i)
            polygons[i] = p[i];
        rulerPoints = r;
    }
    public : double rulerFactor = 1.0;
    public : double woundArea;
    public :~Survey()
    {
        for(QPolygonF *p = polygons.begin(); p!= polygons.end(); ++p)
            p->clear();
        polygons.clear();
        rulerPoints.clear();
    }

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
    public : int type() const override { return DOCTOR_TYPE;}
};

class DatabaseModel
{
    public : QStandardItemModel * model = nullptr;
    public : Doctor* doctor = nullptr;

    public : QIcon patientIcon = QIcon(QStringLiteral(":Ui/Icons/Patient.png"));
    public : QIcon doctorIcon = QIcon(QStringLiteral(":Ui/Icons/Doctor.png"));
    public : QIcon woundIcon = QIcon(QStringLiteral(":Ui/Icons/Wound.png"));
    public : QIcon surveyIcon = QIcon(QStringLiteral(":Ui/Icons/Survey.png"));

    public : DatabaseModel(QObject * parent = 0): model(new QStandardItemModel(parent)){}
    public : void addDoctor(Doctor* newDoctor)
    {
        model->invisibleRootItem()->appendRow(newDoctor);
        doctor = newDoctor;
    }
    public : ~DatabaseModel(){}
};

#endif // DATABASEMODEL_H
