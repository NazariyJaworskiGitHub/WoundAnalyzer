#include "imageinterface.h"

#include "Utilities/mathutilities.h"

ImageInterface::ImageInterface(QWidget *parent):
    QLabel(parent),
    _managementMode(EDIT_MODE)
{
    setAcceptDrops(true);
}

void ImageInterface::openImage(QString fileName)
{
    ImageManager::instance()->openImage(fileName);
    Q_EMIT updateFilenameAndResetZoom_signal(fileName);
    clearAll();
}

QPointF *ImageInterface::_findNodeWithPosInPolygons(const QPointF &pos)
{
    QPointF *ptr = nullptr;
    for(QPolygonF *p = _polygons.begin(); p!= _polygons.end(); ++p)
    {
        bool found = false;
        for(QPointF *n = p->begin(); n!= p->end(); ++n)
            // note that node has radius thickness + 2
            if(MathUtilities::length(*n,pos) < polygonEdgeThickness+3)
            {
                ptr = n;
                _ptrToPolygonWhereNodeIsFound = p;
                found = true;
                break;
            }
        if(found)break;
    }
    return ptr;
}

QPointF *ImageInterface::_findNodeWithPosInRuler(const QPointF &pos)
{
    QPointF *ptr = nullptr;
    // note that node has radius thickness + 2
    if(_rulerPoints.size() == 1)
    {
        if(MathUtilities::length(_rulerPoints[0],pos) < rulerThickness+3)
            ptr = &_rulerPoints[0];
    }
    else if(_rulerPoints.size() > 1)
    {
        if(MathUtilities::length(_rulerPoints[0],pos) < rulerThickness+3)
            ptr = &_rulerPoints[0];
        else if(MathUtilities::length(_rulerPoints[1],pos) < rulerThickness+3)
            ptr = &_rulerPoints[1];
    }
    return ptr;
}

bool ImageInterface::_findLineWithPosInPolygons(
        QPointF **ptrToA,
        QPointF **ptrToB,
        const QPointF &pos)
{
    bool found = false;
    for(QPolygonF *p = _polygons.begin(); p!= _polygons.end(); ++p)
    {
        if(p->size()>1)
        {
            QPointF *n = p->begin();         // begin
            *ptrToA = n;                     // a = first
            for(++n; n!= p->end(); ++n)     // move to next, so a become prev
            {
                *ptrToB = n;                 // b = cur
                if(MathUtilities::isOnSegment(**ptrToA, **ptrToB, pos))
                {
                    found = true;
                    break;
                }
                *ptrToA = n;                 // a = cur
            }
            if(!found)  // check first-last
            {
                *ptrToB =  &p->first();
                *ptrToA =  &p->last();
                if(MathUtilities::isOnSegment(**ptrToA, **ptrToB, pos))
                {
                    found = true;
                    break;
                }
            }
        }
        if(found)
        {
            _ptrToPolygonWhereLineIsFound = p;
            break;
        }
        else
        {
            *ptrToA = nullptr;
            *ptrToB = nullptr;
        }
    }
    return found;
}

void ImageInterface::clearAll()
{
    for(QPolygonF *p = _polygons.begin(); p!= _polygons.end(); ++p)
        p->clear();
    _rulerPoints.clear();
    ImageManager::instance()->rulerFactor = 1.0;
    ImageManager::instance()->rulerLength = 0;
    drawAll();
    Q_EMIT updatePolygonArea_signal(0);
    Q_EMIT updateRulerDistance_signal(0);
}

void ImageInterface::drawAll()
{
    ImageManager::instance()->cleanDrawingLayer();

    Q_EMIT updateRulerDistance_signal(ImageManager::instance()->drawRuler(
                                          _rulerPoints,
                                          rulerColor,
                                          rulerNodesColor,
                                          rulerTextColor,
                                          rulerThickness));

    double area = 0;
    for(auto p: _polygons)
        area += ImageManager::instance()->drawPolygon(
                    p,
                    polygonEdgeColor,
                    polygonColor,
                    polygonTextColor,
                    polygonEdgeThickness);
    Q_EMIT updatePolygonArea_signal(area);

    if(_nodeToHighlight)
    {
        if(_isPolygonNodeHighlighted)
            ImageManager::instance()->highlightCircle(
                        *_nodeToHighlight,
                        polygonEdgeColor,
                        polygonEdgeThickness);
        else
            ImageManager::instance()->highlightCircle(
                        *_nodeToHighlight,
                        rulerNodesColor,
                        rulerThickness);
    }

    if(_lineToHighlightA)
    {
        ImageManager::instance()->highlightLine(
                    *_lineToHighlightA,
                    *_lineToHighlightB,
                    polygonEdgeColor,
                    polygonEdgeThickness);
    }

    this->setPixmap(ImageManager::instance()->getImageAsQPixmap());
}

void ImageInterface::zoom(double delta)
{
    for(QPolygonF *p = _polygons.begin(); p!= _polygons.end(); ++p)
        for(QPointF *n = p->begin(); n!= p->end(); ++n)
            *n *= 1.0 + delta / ImageManager::instance()->zoomFactor;
    for(QPointF *n = _rulerPoints.begin(); n!= _rulerPoints.end(); ++n)
        *n *= 1.0 + delta / ImageManager::instance()->zoomFactor;

    ImageManager::instance()->zoomFactor += delta;

    drawAll();
}

void ImageInterface::mouseMoveEvent(QMouseEvent *ev)
{
    if(ImageManager::instance()->isImageOpened)
    {
        Q_EMIT mouseMoved_signal(ev);   // update at status bar

        if(_managementMode == EDIT_MODE)
        {
            if(_nodeToMove)
            {
                _nodeToMove->setX(ev->x());
                _nodeToMove->setY(ev->y());
            }

            if(_nodeToHighlight)
            {
                // note that node has radius thickness + 2
                if(MathUtilities::length(*_nodeToHighlight,ev->pos()) >= polygonEdgeThickness+3)
                    _nodeToHighlight = nullptr;
            }
            else
            {
                _nodeToHighlight = _findNodeWithPosInPolygons(ev->pos());
                if(_nodeToHighlight)
                    _isPolygonNodeHighlighted = true;
                else
                {
                    _nodeToHighlight = _findNodeWithPosInRuler(ev->pos());
                    if(_nodeToHighlight)
                        _isPolygonNodeHighlighted = false;
                }
            }
            if(!_nodeToHighlight)
            {
                if(_lineToHighlightA)
                {
                    if(!MathUtilities::isOnSegment(
                                *_lineToHighlightA,
                                *_lineToHighlightB,
                                ev->pos()))
                    {
                        _lineToHighlightA = nullptr;
                        _lineToHighlightB = nullptr;
                    }
                }
                else
                    _findLineWithPosInPolygons(&_lineToHighlightA,&_lineToHighlightB,ev->pos());
            }
            drawAll();
        }
    }

}

void ImageInterface::mousePressEvent(QMouseEvent *ev)
{
    if(ImageManager::instance()->isImageOpened)
    {
        if(ev->button() == Qt::LeftButton &&
                ev->x()<=this->pixmap()->size().width() &&
                ev->y()<=this->pixmap()->size().height())
        {
            if(_managementMode == POLYGON_MODE)
            {
                if(createNewPolygon)
                {
                    createNewPolygon = false;
                    QPolygon p;
                    p.append(ev->pos());
                    _polygons.append(p);
                }
                else
                    _polygons.last().append(ev->pos());
                drawAll();
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
                drawAll();
            }
            else if(_managementMode == EDIT_MODE) // move nodes
            {
                // note that node has radius thickness + 2
                _nodeToMove = _findNodeWithPosInPolygons(ev->pos());
                if(!_nodeToMove)
                    _nodeToMove = _findNodeWithPosInRuler(ev->pos());

//                ImageManager::instance()->floodFill(ev->pos());
//                drawAll();
            }
        }
        else if(ev->button() == Qt::RightButton)
        {
            if(_managementMode == EDIT_MODE)
            {
                QPointF *nodeToDelete = _findNodeWithPosInPolygons(ev->pos());
                if(nodeToDelete)
                {
                    _ptrToPolygonWhereNodeIsFound->erase(nodeToDelete);
                    _nodeToMove = nullptr;
                    _nodeToHighlight = nullptr;
                    _lineToHighlightA = nullptr;
                    _lineToHighlightB = nullptr;
                }
                else
                {
                    QPointF *a;
                    QPointF *b;
                    if(_findLineWithPosInPolygons(&a,&b,ev->pos()))
                    {
                        _ptrToPolygonWhereLineIsFound->insert(b,ev->pos());
                        _nodeToMove = nullptr;
                        _nodeToHighlight = nullptr;
                        _lineToHighlightA = nullptr;
                        _lineToHighlightB = nullptr;
                    }
                }
                drawAll();
            }
        }
    }
}

void ImageInterface::mouseReleaseEvent(QMouseEvent *ev)
{
    if(_managementMode == EDIT_MODE) // move nodes
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
    _polygons.clear();
}
