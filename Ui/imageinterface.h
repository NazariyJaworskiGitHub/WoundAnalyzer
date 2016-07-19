#ifndef IMAGEINTERFACE_H
#define IMAGEINTERFACE_H

#include <QLabel>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPoint>
#include <QPolygon>

#include "Image/imagemanager.h"

class ImageInterface : public QLabel
{
    Q_OBJECT

    public : explicit ImageInterface(QWidget *parent = nullptr);

    public : void openImage(QString fileName);
    public : Q_SIGNAL void updateFilename_signal(QString fileName); // see MainWindow

    public : enum ManagementMode{POLYGON_MODE, RULER_MODE, COMMON_MODE};
    private: ManagementMode _managementMode;
    public : bool managementMode() const {return _managementMode;}
    public : void setManagementMode(ManagementMode mode) {
        _managementMode = mode; Q_EMIT updateManagementMode_signal(mode);}
    public : Q_SIGNAL void updateManagementMode_signal(
            ImageInterface::ManagementMode mode);                   // see MainWindow

    private: QPolygon _polygon;
    private: QPolygon _rulerPoints;
    public : void clearStuff();

    public : void mouseMoveEvent(QMouseEvent *ev) override;
    public : Q_SIGNAL void mouseMoved_signal(QMouseEvent *ev);      // see MainWindow
    public : void mousePressEvent(QMouseEvent *ev) override;
    public : Q_SIGNAL void updatePolygonArea_signal(int area);      // see MainWindow
    public : Q_SIGNAL void updateRulerDistance_signal(double distance);// see MainWindow

    public : void dragEnterEvent(QDragEnterEvent *ev) override;
    public : void dropEvent(QDropEvent *ev) override;

    public : ~ImageInterface();
};

#endif // IMAGEINTERFACE_H
