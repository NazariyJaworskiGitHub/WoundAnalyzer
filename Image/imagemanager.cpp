#include "imagemanager.h"

void ImageManager::cleanPolygonStencil()
{
    _myPolygonImage = cv::Mat(_myImage.rows, _myImage.cols, _myImage.type());
    //_myPolygonImage = cv::Mat(_myImage.rows, _myImage.cols, CV_8UC1);
}

void ImageManager::cleanRulerStencil()
{
    _myRulerImage = cv::Mat(_myImage.rows, _myImage.cols, _myImage.type());
    //_myPolygonImage = cv::Mat(_myImage.rows, _myImage.cols, CV_8UC1);
}

int ImageManager::drawPolygon(const QPolygon &polygon)
{
    int _area = 0;
    if(polygon.size() != 0)
    {
        std::vector<cv::Point> _tmpPoly;
        _tmpPoly.resize(polygon.size());
        for(int i=0; i<polygon.size(); ++i)
        {
            _tmpPoly[i].x = polygon[i].x();
            _tmpPoly[i].y = polygon[i].y();
        }
        if(_tmpPoly.size()==1)
            cv::circle(_myPolygonImage,_tmpPoly[0],1,cv::Scalar(127,127,127));
        else if(_tmpPoly.size()==2)
            cv::polylines(_myPolygonImage, _tmpPoly, false, cv::Scalar(255,255,255));
        else
        {
            std::vector<std::vector<cv::Point>> _p(1);
            _p[0] = _tmpPoly;
            cv::fillPoly(_myPolygonImage,_p,cv::Scalar(127,127,127));
            cv::polylines(_myPolygonImage, _tmpPoly, true, cv::Scalar(255,255,255));
            _area = cv::contourArea(_tmpPoly); // TEST IT it uses Green formula and returns double!
            cv::putText(
                        _myPolygonImage,
                        QString::number(_area).toStdString(),
                         _tmpPoly[0],
                        cv::FONT_HERSHEY_PLAIN,
                        1.0,
                        cv::Scalar(127,255,127));
        }
        _tmpPoly.clear();
    }
    return _area;
}

double ImageManager::drawRuler(const QPolygon &ruler)
{
    double _distance = 0;
    if(ruler.size() != 0)
    {
        cv::circle(_myRulerImage,cv::Point(ruler[0].x(),ruler[0].y()),1,cv::Scalar(255,255,255));
        cv::circle(_myRulerImage,cv::Point(ruler[0].x(),ruler[0].y()),3,cv::Scalar(0,0,255),2);
        if(ruler.size()==2)
        {
            cv::line(
                    _myRulerImage,
                    cv::Point(ruler[0].x(),ruler[0].y()),
                    cv::Point(ruler[1].x(),ruler[1].y()),
                    cv::Scalar(255,0,0),
                    1);
            cv::circle(_myRulerImage,cv::Point(ruler[1].x(),ruler[1].y()),1,cv::Scalar(255,255,255));
            cv::circle(_myRulerImage,cv::Point(ruler[1].x(),ruler[1].y()),3,cv::Scalar(0,0,255),2);
            _distance = std::sqrt((ruler[0].x()-ruler[1].x())*(ruler[0].x()-ruler[1].x()) +
                    (ruler[0].y()-ruler[1].y())*(ruler[0].y()-ruler[1].y()));

            cv::putText(
                        _myRulerImage,
                        QString::number(_distance).toStdString(),
                        cv::Point(
                            ruler[0].x() + (ruler[1].x() - ruler[0].x())/2.0,
                            ruler[0].y() + (ruler[1].y() - ruler[0].y())/2.0),
                        cv::FONT_HERSHEY_PLAIN,
                        1.0,
                        cv::Scalar(0,255,255));
        }
    }
    return _distance;
}

const QPixmap ImageManager::getImageAsQPixmap() const
{
    cv::Mat _result;
    cv::addWeighted(_myImage, 0.5, _myPolygonImage, 0.5, 0.0, _result);
    cv::add(_result, _myRulerImage, _result);
    return QPixmap::fromImage(Mat2QImage(_result));
}

ImageManager::ImageManager(QObject *parent):
    QObject(parent)
{

}

void ImageManager::openImage(QString fileName)
{
    _myImage = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);
    cleanPolygonStencil();
    cleanRulerStencil();
}

QImage ImageManager::Mat2QImage(cv::Mat const& src)
{
//    QImage dest = QImage(
//                (unsigned char*) src.data,
//                src.cols,
//                src.rows,
//                src.step,
//                QImage::Format_RGB888);

    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);

    const float scale = 255.0;

    if (src.depth() == CV_8U) {
        if (src.channels() == 1) {
            for (int i = 0; i < src.rows; ++i) {
                for (int j = 0; j < src.cols; ++j) {
                    int level = src.at<quint8>(i, j);
                    dest.setPixel(j, i, qRgb(level, level, level));
                }
            }
        } else if (src.channels() == 3) {
            for (int i = 0; i < src.rows; ++i) {
                for (int j = 0; j < src.cols; ++j) {
                    cv::Vec3b bgr = src.at<cv::Vec3b>(i, j);
                    dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
                }
            }
        }
    } else if (src.depth() == CV_32F) {
        if (src.channels() == 1) {
            for (int i = 0; i < src.rows; ++i) {
                for (int j = 0; j < src.cols; ++j) {
                    int level = scale * src.at<float>(i, j);
                    dest.setPixel(j, i, qRgb(level, level, level));
                }
            }
        } else if (src.channels() == 3) {
            for (int i = 0; i < src.rows; ++i) {
                for (int j = 0; j < src.cols; ++j) {
                    cv::Vec3f bgr = scale * src.at<cv::Vec3f>(i, j);
                    dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
                }
            }
        }
    }

    return dest;
}

ImageManager::~ImageManager()
{

}

ImageManager *ImageManager::instance()
{
    static ImageManager _instanceOfImageManager;
    return &_instanceOfImageManager;
}

