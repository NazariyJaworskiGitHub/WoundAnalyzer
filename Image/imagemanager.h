#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QPolygon>
#include <opencv2/opencv.hpp>


/// Manages image
/// \todo move it all to ImageInterface
class ImageManager : public QObject
{
    Q_OBJECT

    public : bool isImageOpened = false;

    private: cv::Mat _myImage;
    private: cv::Mat _myDrawingLayer;

    public : double drawingLayerTransparency = 0.75;

    public : void cleanDrawingLayer();

    public : void highlightCircle(
            const QPoint &p,
            const QColor &col,
            int radius); // note that node has radius thickness + 4

    public : void highlightLine(
            const QPoint &a,
            const QPoint &b,
            const QColor &col,
            int thickness); // note that line has thickness thickness + 2

    public : int drawPolygon( // returns the area of polygon
            const QPolygon &polygon,
            const QColor &pec,
            const QColor &pc,
            const QColor &pt,
            int thickness); // note that node has radius thickness + 2

    public : double drawRuler( // returns the length of the ruler
            const QPolygon &ruler,
            const QColor &rec,
            const QColor &rc,
            const QColor &rt,
            int thickness); // note that node has radius thickness + 2

    private: cv::Mat _blendLayers() const;

    public : const QPixmap getImageAsQPixmap() const;

    public : void openImage(QString fileName);
    public : void saveImage(QString fileName) const;

    public : static QImage Mat2QImage(cv::Mat const& src);

    private: ImageManager(QObject *parent = nullptr);
    private: ~ImageManager();

    public : static ImageManager *instance();
};

#endif // IMAGEMANAGER_H
