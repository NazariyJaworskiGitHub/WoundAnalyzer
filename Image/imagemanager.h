#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QPolygon>
#include <opencv2/opencv.hpp>

/// Manages image
class ImageManager : public QObject
{
    Q_OBJECT

    private: QString _fileName;
    public : QString getFilename() const {return _fileName;}
    private: cv::Mat _myImage;
    private: cv::Mat _myPolygonImage;
    public : void cleanPolygonStencil();
    public : void drawPolygon(const QPolygon &polygon);
    private: std::vector<int> _polygonAreas;
    public : const std::vector<int> & getPolygonAreas() const {return _polygonAreas;}
    public : const cv::Mat & getImage() const {return _myImage;}
    public : const QPixmap getImageAsQPixmap() const;

    private: ImageManager(QObject *parent = nullptr);

    public : void openImage(QString fileName) throw(std::runtime_error);
    public : static QImage Mat2QImage(cv::Mat const& src);

    private: ~ImageManager();

    public : static ImageManager *instance();
};

#endif // IMAGEMANAGER_H
