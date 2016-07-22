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
    public : int drawPolygon( // returns the area of polygon
            const QPolygon &polygon,
            const QColor &pec = QColor(255, 255, 255),
            const QColor &pc = QColor(127, 127, 127),
            const QColor &pt = QColor(127, 255, 127),
            int thickness = 1); // note that node has radius thickness + 2
    public : double drawRuler( // returns the area of polygon
            const QPolygon &ruler,
            const QColor &rec = QColor(255,   0,   0),
            const QColor &rc = QColor(  0,   0, 255),
            const QColor &rt = QColor(  0, 255, 255),
            int thickness = 3); // note that node has radius thickness + 2
    public : const cv::Mat & getImage() const {return _myImage;}
    public : const QPixmap getImageAsQPixmap(int transparency = 50) const;

    private: ImageManager(QObject *parent = nullptr);

    public : void openImage(QString fileName);
    public : void saveImage(QString fileName,int transparency = 50) const;
    public : static QImage Mat2QImage(cv::Mat const& src);

    private: ~ImageManager();

    public : static ImageManager *instance();
};

#endif // IMAGEMANAGER_H
