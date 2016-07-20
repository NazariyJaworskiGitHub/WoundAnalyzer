#include "imageinterface.h"

ImageInterface::ImageInterface(QWidget *parent):
    QLabel(parent),
    _managementMode(COMMON_MODE)
{
    setAcceptDrops(true);
}

void ImageInterface::openImage(QString fileName)
{
    ImageManager::instance()->openImage(fileName);
    Q_EMIT updateFilename_signal(fileName);
    clearStuff();
}

void ImageInterface::clearStuff()
{
    _polygon.clear();
    _rulerPoints.clear();
    drawStuff();
    Q_EMIT updatePolygonArea_signal(0);
    Q_EMIT updateRulerDistance_signal(0);
}

void ImageInterface::drawStuff()
{
    ImageManager::instance()->cleanPolygonStencil();
    ImageManager::instance()->cleanRulerStencil();
    Q_EMIT updatePolygonArea_signal(ImageManager::instance()->drawPolygon(
                                        _polygon,
                                        polygonEdgeColor,
                                        polygonColor,
                                        polygonTextColor,
                                        polygonEdgeThickness));
    Q_EMIT updateRulerDistance_signal(ImageManager::instance()->drawRuler(
                                          _rulerPoints,
                                          rulerColor,
                                          rulerNodesColor,
                                          rulerTextColor,
                                          rulerThickness));
    this->setPixmap(ImageManager::instance()->getImageAsQPixmap(transparency));
}

void ImageInterface::mouseMoveEvent(QMouseEvent *ev)
{
    Q_EMIT mouseMoved_signal(ev);
}

void ImageInterface::mousePressEvent(QMouseEvent *ev)
{
    if(_managementMode == POLYGON_MODE)
    {
        _polygon.append(ev->pos());
        drawStuff();
    }
    else if(_managementMode == RULER_MODE)
    {
        if(_rulerPoints.size() == 0)
            _rulerPoints.append(ev->pos());
        else if(_rulerPoints.size() == 1)
            _rulerPoints.append(ev->pos());
        else if(_rulerPoints.size() == 2)
        {
            _rulerPoints[0] = _rulerPoints[1];
            _rulerPoints[1] = ev->pos();
        }
        drawStuff();
    }
}

void ImageInterface::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasUrls())
        ev->acceptProposedAction();
}

void ImageInterface::dropEvent(QDropEvent *ev)
{
    openImage(ev->mimeData()->urls().first().toLocalFile());
}

ImageInterface::~ImageInterface()
{
    _polygon.clear();
}

