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

    private: cv::Mat _myImage;
    private: cv::Mat _myPolygonImage;
    private: cv::Mat _myRulerImage;
    public : void cleanPolygonStencil();
    public : void cleanRulerStencil();
    public : int drawPolygon(
            const QPolygon &polygon,
            const QColor &pec,
            const QColor &pc,
            const QColor &pt,
            int thickness); // returns the area of polygon
    public : double drawRuler(
            const QPolygon &ruler,
            const QColor &rec,
            const QColor &rc,
            const QColor &rt,
            int thickness); // returns the area of polygon
    public : const cv::Mat & getImage() const {return _myImage;}
    public : const QPixmap getImageAsQPixmap(int transparency = 50) const;

    private: ImageManager(QObject *parent = nullptr);

    public : void openImage(QString fileName);
    public : static QImage Mat2QImage(cv::Mat const& src);

    private: ~ImageManager();

    public : static ImageManager *instance();
};

#endif // IMAGEMANAGER_H
