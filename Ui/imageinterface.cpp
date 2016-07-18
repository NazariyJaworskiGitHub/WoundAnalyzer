#include "imageinterface.h"
#include <iostream>
ImageInterface::ImageInterface(QWidget *parent):QLabel(parent)
{
    setAcceptDrops(true);
}

void ImageInterface::openImage(QString fileName)
{
    _polygon.clear();
    ImageManager::instance()->openImage(fileName);
    this->setPixmap(ImageManager::instance()->getImageAsQPixmap());
}

void ImageInterface::cleanPolygon()
{
    _polygon.clear();
    ImageManager::instance()->cleanPolygonStencil();
    this->setPixmap(ImageManager::instance()->getImageAsQPixmap());
}

void ImageInterface::mousePressEvent(QMouseEvent *ev)
{
    if(_isPolygonMode)
    {
        _polygon.append(ev->pos());

        if(_polygon.size()>1)
        {
            ImageManager::instance()->cleanPolygonStencil();
            ImageManager::instance()->drawPolygon(_polygon);
            this->setPixmap(ImageManager::instance()->getImageAsQPixmap());
        }
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

