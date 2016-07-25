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
    public : Q_SIGNAL void updateFilename_signal(QString fileName);

    public : enum ManagementMode{POLYGON_MODE, RULER_MODE, EDIT_MODE};
    private: ManagementMode _managementMode;
    public : bool managementMode() const {return _managementMode;}
    public : void setManagementMode(ManagementMode mode) {
        _managementMode = mode; Q_EMIT updateManagementMode_signal(mode);}
    public : Q_SIGNAL void updateManagementMode_signal(
            ImageInterface::ManagementMode mode);

    public : QColor polygonEdgeColor            = QColor(255, 255, 255);
    public : QColor polygonColor                = QColor(127, 127, 127);
    public : QColor polygonTextColor            = QColor(127, 255, 127);
    public : int    polygonEdgeThickness        = 1;
    public : QColor rulerColor                  = QColor(255,   0,   0);
    public : QColor rulerNodesColor             = QColor(255, 255,   0);
    public : QColor rulerTextColor              = QColor(  0, 255, 255);
    public : int    rulerThickness              = 1;

    private: QVector<QPolygon> _polygons;
    public : bool createNewPolygon = true;
    private: QPolygon _rulerPoints;             /// \todo make it QLine

    private: QPoint *_nodeToMove = nullptr;

    private: QPoint *_nodeToHighlight = nullptr;
    private: bool _isPolygonNodeHighlighted = false;    // or it is ruler node?
    private: QPoint * _lineToHighlightA = nullptr;
    private: QPoint * _lineToHighlightB = nullptr;
    private: QPolygon *_ptrToPolygonWhereNodeIsFound = nullptr;
    private: QPolygon *_ptrToPolygonWhereLineIsFound = nullptr;

    private: QPoint *_findNodeWithPosInPolygons(const QPoint &pos);
    private: QPoint *_findNodeWithPosInRuler(const QPoint &pos);

    private: bool _findLineWithPosInPolygons(
            QPoint **ptrToA,
            QPoint **ptrToB,
            const QPoint &pos);

    public : void clearAll();
    public : void drawAll();

    public : void mouseMoveEvent(QMouseEvent *ev) override;
    public : void mousePressEvent(QMouseEvent *ev) override;
    public : void mouseReleaseEvent(QMouseEvent *ev) override;

    public : Q_SIGNAL void mouseMoved_signal(QMouseEvent *ev);
    public : Q_SIGNAL void updatePolygonArea_signal(double area);
    public : Q_SIGNAL void updateRulerDistance_signal(double distance);

    public : void dragEnterEvent(QDragEnterEvent *ev) override;
    public : void dropEvent(QDropEvent *ev) override;

    public : ~ImageInterface();

    public : void deletePolygonNode(QPoint node);
};

#endif // IMAGEINTERFACE_H
