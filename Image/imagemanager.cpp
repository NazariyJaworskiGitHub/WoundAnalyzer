#include "imagemanager.h"

void ImageManager::cleanPolygonStencil()
{
    _myPolygonImage = cv::Mat(_myImage.rows, _myImage.cols, _myImage.type());
    //_myPolygonImage = cv::Mat(_myImage.rows, _myImage.cols, CV_8UC1);
}

void ImageManager::drawPolygon(const QPolygon &polygon)
{
    std::vector<cv::Point> _tmpPoly;
    _tmpPoly.resize(polygon.size());
    for(int i=0; i<polygon.size(); ++i)
    {
        _tmpPoly[i].x = polygon[i].x();
        _tmpPoly[i].y = polygon[i].y();
    }
    if(_tmpPoly.size()==1)
        cv::circle(_myPolygonImage,_tmpPoly[0],1,cv::Scalar(255,255,255));
    else if(_tmpPoly.size()==2)
        cv::polylines(_myPolygonImage, _tmpPoly, false, cv::Scalar(255,255,255));
    else
    {
        std::vector<std::vector<cv::Point>> _p(1);
        _p[0] = _tmpPoly;
        cv::fillPoly(_myPolygonImage,_p,cv::Scalar(255,255,255));
        std::cout << "Polygon area = " << cv::contourArea(_tmpPoly) << " sq pix" << std::endl;
    }
    _tmpPoly.clear();
}

const QPixmap ImageManager::getImageAsQPixmap() const
{
    cv::Mat _result;
    cv::addWeighted(_myImage, 0.5, _myPolygonImage, 0.5, 0.0, _result);
    return QPixmap::fromImage(Mat2QImage(_result));
}

ImageManager::ImageManager(QObject *parent):
    QObject(parent)
{

}

void ImageManager::openImage(QString fileName) throw(std::runtime_error)
{
    _fileName = fileName;
    _myImage = cv::imread(_fileName.toStdString());
    cleanPolygonStencil();
}

QImage ImageManager::Mat2QImage(cv::Mat const& src)
{
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

