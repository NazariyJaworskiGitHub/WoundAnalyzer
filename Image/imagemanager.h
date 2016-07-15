#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>

/// Manages image
class ImageManager : public QObject
{
    Q_OBJECT

    private: cv::Mat _myImage;
    public : const cv::Mat & getImage() const {return _myImage;}
    public : const QPixmap getImageAsQPixmap() const {
        return QPixmap::fromImage(Mat2QImage(_myImage));}

    private: ImageManager(QObject *parent = nullptr);

    public : void openImage(QString fileName) throw(std::runtime_error);
    public : static QImage Mat2QImage(cv::Mat const& src);

    private: ~ImageManager();

    public : static ImageManager *instance();
};

#endif // IMAGEMANAGER_H
