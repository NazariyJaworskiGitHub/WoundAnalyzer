#include "imagemanager.h"

void ImageManager::cleanDrawingLayer()
{
    _myDrawingLayer = cv::Mat(_myImage.rows, _myImage.cols, _myImage.type());
    cv::resize(_myDrawingLayer,_myDrawingLayer,cv::Size(),zoomFactor,zoomFactor);
}

void ImageManager::highlightCircle(const QPointF &p, const QColor &col, int radius)
{
    cv::circle(
                _myDrawingLayer,
                cv::Point(p.x(),p.y()),
                radius+4,
                cv::Scalar(col.blue(),col.green(),col.red()),
                CV_FILLED);
}

void ImageManager::highlightLine(
        const QPointF &a, const QPointF &b, const QColor &col, int thickness)
{
    cv::line(
            _myDrawingLayer,
            cv::Point(a.x(),a.y()),
            cv::Point(b.x(),b.y()),
            cv::Scalar(col.blue(),col.green(),col.red()),
            thickness+2);
}

double ImageManager::drawPolygon(
        const QPolygonF &polygon,
        const QColor &pec,
        const QColor &pc,
        const QColor &pt,
        int thickness,
        bool drawText)
{
    double _area = 0;
    if(polygon.size() != 0)
    {
        std::vector<cv::Point> _tmpPoly;
        _tmpPoly.resize(polygon.size());
        for(int i=0; i<polygon.size(); ++i)
        {
            _tmpPoly[i].x = polygon[i].x();
            _tmpPoly[i].y = polygon[i].y();
            cv::circle(
                        _myDrawingLayer,
                        _tmpPoly[i],
                        thickness+2,
                        cv::Scalar(pec.blue(),pec.green(),pec.red()));
        }
        if(_tmpPoly.size() == 2)
            cv::polylines(
                        _myDrawingLayer,
                        _tmpPoly,
                        false,
                        cv::Scalar(pec.blue(),pec.green(),pec.red()),
                        thickness);
        else if(_tmpPoly.size() > 2)
        {
            std::vector<std::vector<cv::Point>> _p(1);
            _p[0] = _tmpPoly;
            cv::fillPoly(
                        _myDrawingLayer,
                        _p,
                        cv::Scalar(pc.blue(),pc.green(),pc.red()));
            cv::polylines(
                        _myDrawingLayer,
                        _tmpPoly,
                        true,
                        cv::Scalar(pec.blue(),pec.green(),pec.red()),
                        thickness);
            /// \todo TEST IT
            if(rulerLength != 0)
                _area = cv::contourArea(_tmpPoly) *
                        (rulerFactor / rulerLength) *
                        (rulerFactor / rulerLength);
            else _area = cv::contourArea(_tmpPoly);
            if(drawText)
                cv::putText(
                        _myDrawingLayer,
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
        const QPolygonF &ruler,
        const QColor &rec,
        const QColor &rc,
        const QColor &rt,
        int thickness,
        bool drawText)
{
    double _distance = 0;
    if(ruler.size() != 0)
    {
        cv::circle(
                    _myDrawingLayer,
                    cv::Point(ruler[0].x(),ruler[0].y()),
                thickness+2,
                cv::Scalar(rc.blue(),rc.green(),rc.red()));
        if(ruler.size()==2)
        {
            cv::line(
                    _myDrawingLayer,
                    cv::Point(ruler[0].x(),ruler[0].y()),
                    cv::Point(ruler[1].x(),ruler[1].y()),
                    cv::Scalar(rec.blue(),rec.green(),rec.red()),
                    thickness);
            cv::circle(
                        _myDrawingLayer,
                        cv::Point(ruler[1].x(),ruler[1].y()),
                    thickness+2,
                    cv::Scalar(rc.blue(),rc.green(),rc.red()));
            _distance = std::sqrt((ruler[0].x()-ruler[1].x())*(ruler[0].x()-ruler[1].x()) +
                    (ruler[0].y()-ruler[1].y())*(ruler[0].y()-ruler[1].y()));

            if(drawText)
                cv::putText(
                        _myDrawingLayer,
                        QString::number(_distance).toStdString() + "px",
                        cv::Point(
                            ruler[0].x() + (ruler[1].x() - ruler[0].x())/2.0,
                            ruler[0].y() + (ruler[1].y() - ruler[0].y())/2.0),
                        cv::FONT_HERSHEY_PLAIN,
                        1.0,
                        cv::Scalar(rt.blue(),rt.green(),rt.red()));
        }
    }
    rulerLength = _distance;
    return _distance;
}

void ImageManager::applyFiltration()
{
//    cv::Mat _result = _myImage.clone();
    cv::Mat _result;
    cv::cvtColor(_myImage, _result, cv::COLOR_BGR2GRAY);
//    cv::resize(_myImage,_result,cv::Size(),zoomFactor,zoomFactor);
    for(int i=0; i<filterIterations; ++i)
    {
        cv::Mat _tmp;
        cv::bilateralFilter(_result,_tmp,0,filterFactorA,filterFactorB);
        cv::GaussianBlur(_tmp,_tmp,cv::Size(5,5),filterFactorC);
        _result = _tmp.clone();
    }
    _myFilteredImage = _result.clone();
}

void ImageManager::floodFill(QPoint p)
{
    cv::floodFill(_myFilteredImage,cv::Point(p.x(),p.y()),cv::Scalar(255,255,255),0,
                  cv::Scalar(rulerFactor,rulerFactor,rulerFactor),
                  cv::Scalar(rulerFactor,rulerFactor,rulerFactor));
}

cv::Mat ImageManager::_blendLayers() const
{
    cv::Mat _result;
    cv::resize(_myFilteredImage,_result,cv::Size(),zoomFactor,zoomFactor);

//    cv::Mat _mask = _result & _myDrawingLayer;
//    cv::Mat _binaryMask;
//    cv::threshold(_mask,_binaryMask,1,256,cv::THRESH_BINARY);

    cv::addWeighted(
                _result,
                1.0 - drawingLayerTransparency,
                _myDrawingLayer,
                drawingLayerTransparency,
                0.0,
                _result);

//    cv::Mat _tmp;
//    cv::cvtColor(_result, _tmp, cv::COLOR_BGR2GRAY);
//    cv::Canny(_tmp, _tmp, rulerFactor*100, drawingLayerTransparency*100);

//    _result = _tmp.clone();
//    std::vector<std::vector<cv::Point>> contours;
//    cv::findContours(_tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//    for(size_t k = 0; k < contours.size(); ++k)
//        cv::approxPolyDP(contours[k], contours[k], 3, true);
//    cv::drawContours(_result, contours, -1, cv::Scalar(255,255,255), 1);

    return _result;
}

const QPixmap ImageManager::getImageAsQPixmap() const
{
    return QPixmap::fromImage(Mat2QImage(_blendLayers()));
}

const QImage ImageManager::getImageAsQImage() const
{
    return Mat2QImage(_blendLayers());
}

void ImageManager::_onLoadImageCleanup()
{
    if(!_myImage.empty())
    {
        cleanDrawingLayer();
        isImageOpened = true;
        zoomFactor = 1.0;
        rulerFactor = 1.0;
        rulerLength = 0;
        _myFilteredImage = _myImage.clone();

        Log::StaticLogger::instance() << "[Image] image is loaded\n";
    }
    else
        Log::StaticLogger::instance() << "[Image] <FAIL> image is not loaded\n";
}

void ImageManager::openImage(const cv::Mat &image)
{
    _myImage = image.clone();
    _onLoadImageCleanup();

}

void ImageManager::openImage(const QString &fileName)
{
    Log::StaticLogger::instance() << "[Image] opening " + fileName.toStdString() + "\n";
    _myImage = cv::imread(fileName.toLocal8Bit().constData(), cv::IMREAD_COLOR);
    _onLoadImageCleanup();
}

void ImageManager::saveImage(const QString &fileName) const
{
    Log::StaticLogger::instance() << "[Image] saving " + fileName.toStdString() + "\n";

    bool _result = cv::imwrite(fileName.toLocal8Bit().constData(),_blendLayers());

    if(_result)
        Log::StaticLogger::instance() << "[Image] image is saved\n";
    else
        Log::StaticLogger::instance() << "[Image] <FAIL> image is not saved\n";
}

QImage ImageManager::Mat2QImage(const cv::Mat &src)
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

ImageManager::ImageManager(QObject *parent): QObject(parent){}

ImageManager::~ImageManager(){}

ImageManager *ImageManager::instance()
{
    static ImageManager _instanceOfImageManager;
    return &_instanceOfImageManager;
}

