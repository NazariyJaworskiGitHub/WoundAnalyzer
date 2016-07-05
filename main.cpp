#include "mainwindow.h"
#include <QApplication>
#include <stdexcept>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    cv::VideoCapture videoCapture(0);
    if(!videoCapture.isOpened())  throw (std::runtime_error("camera error\n"));

    cv::namedWindow("VideoInput",1);
    for(;;)
    {

        cv::Mat frame;
        cv::Mat edges;
        videoCapture >> frame; // get a new frame from camera
        if(!frame.empty())
        {
            cv::cvtColor(frame, edges, CV_BGR2GRAY);
            cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
            cv::Canny(edges, edges, 0, 30, 3);
            imshow("VideoInput", edges);
        }
        if(cv::waitKey(30) >= 0) break;
    }
    return 0;
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
