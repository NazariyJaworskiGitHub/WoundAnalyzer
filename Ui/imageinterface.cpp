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
    if(_managementMode == COMMON_MODE)
    {
        if(_nodeToMove) // move nodes
        {
            _nodeToMove->setX(ev->x());
            _nodeToMove->setY(ev->y());
            drawStuff();
        }
    }
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
    else if(_managementMode == COMMON_MODE) // move nodes
    {
        for(QPoint *n = _polygon.begin(); n!= _polygon.end(); ++n)
            if(std::sqrt( // note that node has radius thickness + 2
                        (n->x()-ev->x())*(n->x()-ev->x()) +
                        (n->y()-ev->y())*(n->y()-ev->y())) < polygonEdgeThickness+3)
            {
                _nodeToMove = n;
                break;
            }
        if(!_nodeToMove)
            for(QPoint *n = _rulerPoints.begin(); n!= _rulerPoints.end(); ++n)
                if(std::sqrt( // note that node has radius thickness + 2
                            (n->x()-ev->x())*(n->x()-ev->x()) +
                            (n->y()-ev->y())*(n->y()-ev->y())) < rulerThickness+3)
                {
                    _nodeToMove = n;
                    break;
                }
    }
}

void ImageInterface::mouseReleaseEvent(QMouseEvent *ev)
{
    if(_managementMode == COMMON_MODE) // move nodes
    {
        _nodeToMove = nullptr;
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

