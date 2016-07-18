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

    private: bool _isPolygonMode = false;
    public : bool polygonMode() const {return _isPolygonMode;}
    public : void setPolygonMode(bool mode) {_isPolygonMode = mode;}
    public : void switchPolygonMode() {_isPolygonMode = !_isPolygonMode;}

    private: QPolygon _polygon;
    public : void cleanPolygon();

    public : void mousePressEvent(QMouseEvent *ev) override;

    public : void dragEnterEvent(QDragEnterEvent *ev) override;
    public : void dropEvent(QDropEvent *ev) override;

    public : ~ImageInterface();
};

#endif // IMAGEINTERFACE_H
