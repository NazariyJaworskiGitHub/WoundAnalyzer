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

int ImageManager::drawPolygon(
        const QPolygon &polygon,
        const QColor &pec,
        const QColor &pc,
        const QColor &pt,
        int thickness)
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
            cv::circle(_myPolygonImage,_tmpPoly[0],thickness,cv::Scalar(pec.blue(),pec.green(),pec.red()));
        else if(_tmpPoly.size()==2)
            cv::polylines(_myPolygonImage, _tmpPoly, false, cv::Scalar(pec.blue(),pec.green(),pec.red()),thickness);
        else
        {
            std::vector<std::vector<cv::Point>> _p(1);
            _p[0] = _tmpPoly;
            cv::fillPoly(_myPolygonImage,_p,cv::Scalar(pc.blue(),pc.green(),pc.red()));
            cv::polylines(_myPolygonImage, _tmpPoly, true, cv::Scalar(pec.blue(),pec.green(),pec.red()),thickness);
            _area = cv::contourArea(_tmpPoly); // TEST IT it uses Green formula and returns double!
            cv::putText(
                        _myPolygonImage,
                        QString::number(_area).toStdString(),
                         _tmpPoly[0],
                        cv::FONT_HERSHEY_PLAIN,
                        1.0,
                        cv::Scalar(pt.blue(),pt.green(),pt.red()));
        }
        _tmpPoly.clear();
    }
    return _area;
}

double ImageManager::drawRuler(
        const QPolygon &ruler,
        const QColor &rec,
        const QColor &rc,
        const QColor &rt,
        int thickness)
{
    double _distance = 0;
    if(ruler.size() != 0)
    {
//        cv::circle(_myRulerImage,cv::Point(ruler[0].x(),ruler[0].y()),1,cv::Scalar(255,255,255));
        cv::circle(_myRulerImage,cv::Point(ruler[0].x(),ruler[0].y()),3,cv::Scalar(rc.blue(),rc.green(),rc.red()),2);
        if(ruler.size()==2)
        {
            cv::line(
                    _myRulerImage,
                    cv::Point(ruler[0].x(),ruler[0].y()),
                    cv::Point(ruler[1].x(),ruler[1].y()),
                    cv::Scalar(rec.blue(),rec.green(),rec.red()),
                    thickness);
//            cv::circle(_myRulerImage,cv::Point(ruler[1].x(),ruler[1].y()),1,cv::Scalar(255,255,255));
            cv::circle(_myRulerImage,cv::Point(ruler[1].x(),ruler[1].y()),3,cv::Scalar(rc.blue(),rc.green(),rc.red()),2);
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
                        cv::Scalar(rt.blue(),rt.green(),rt.red()));
        }
    }
    return _distance;
}

const QPixmap ImageManager::getImageAsQPixmap(int transparency) const
{
    cv::Mat _result;
    cv::addWeighted(_myImage, 1.0 - transparency/100.0, _myPolygonImage, transparency/100.0, 0.0, _result);
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

