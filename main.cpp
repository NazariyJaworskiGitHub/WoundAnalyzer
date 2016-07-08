#include "mainwindow.h"
#include <QApplication>
#include <stdexcept>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat frame;
cv::Mat edges;
bool start = false;
cv::Point oldP;
int redThreshold[2] = {0,256};
int greenThreshold[2] = {0,256};
int blueThreshold[2] = {0,256};

static void onMouse( int event, int x, int y, int, void* )
{
    if( event != cv::EVENT_LBUTTONDOWN)
        return;

    if(!start)
    {
       oldP = cv::Point(x,y);
       start = true;
    }
    else
    {
        cv::line(edges,cv::Point(x,y),oldP,cv::Scalar(255));
        oldP = cv::Point(x,y);
    }

//    cv::floodFill(frame, edges, cv::Point(x,y), cv::Scalar(255), 0,
//                  cv::Scalar(redThreshold[0], greenThreshold[0], blueThreshold[0]),
//                  cv::Scalar(redThreshold[1], greenThreshold[1], blueThreshold[1]),
//            cv::FLOODFILL_MASK_ONLY);
}

int main(int argc, char *argv[])
{
    cv::VideoCapture videoCapture(0);
    if(!videoCapture.isOpened())  throw (std::runtime_error("camera error\n"));

    cv::namedWindow("Video Input",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Input Histogram", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Video Input",onMouse,0);


    cv::createTrackbar("Red min", "Input Histogram", &redThreshold[0], 256);
    cv::createTrackbar("Red max", "Input Histogram", &redThreshold[1], 256);
    cv::createTrackbar("Green min", "Input Histogram", &greenThreshold[0], 256);
    cv::createTrackbar("Green max", "Input Histogram", &greenThreshold[1], 256);
    cv::createTrackbar("Blue min", "Input Histogram", &blueThreshold[0], 256);
    cv::createTrackbar("Blue max", "Input Histogram", &blueThreshold[1], 256);

    std::vector<cv::Mat> frameSeq(3);
    std::vector<cv::Mat> framePlanes(3);
    std::vector<cv::Mat> histPlanes(3);
    std::vector<cv::Mat> edgePlanes(3);

    std::vector<std::vector<cv::Point> > contours;

//    int dims = 1;
//    const int sizes[] = {256};
//    const int channels[] = {0};
//    float range[] = {0,256};
//    const float *ranges[] = {range};

    for(;;)
    {
        videoCapture >> frame; // get a new frame from camera
        if(!frame.empty())
        {
            if(edges.empty())
                edges = cv::Mat(frame.rows, frame.cols, CV_8UC1);
//            if(frameSeq[0].empty()){
//                frameSeq[0] = frame.clone();
//                continue;}
//            if(frameSeq[1].empty()){
//                frameSeq[1] = frame.clone();
//                continue;}
//            if(frameSeq[2].empty()){
//                frameSeq[2] = frame.clone();
//                continue;}

//            frameSeq[0] = frameSeq[1].clone();
//            frameSeq[1] = frameSeq[2].clone();
//            frameSeq[2] = frame.clone();

//            cv::fastNlMeansDenoisingColoredMulti(frameSeq,edges,1,3,1,1,3,3);

//            cv::medianBlur(frame,frame,3);
//            cv::GaussianBlur(frame,frame,cv::Size(5,5),0);

//            cv::split(frame, framePlanes); //split int R G B

//            cv::calcHist(&framePlanes[0], 1, channels, cv::Mat(), histPlanes[0], dims, sizes, ranges);
//            cv::calcHist(&framePlanes[1], 1, channels, cv::Mat(), histPlanes[1], dims, sizes, ranges);
//            cv::calcHist(&framePlanes[2], 1, channels, cv::Mat(), histPlanes[2], dims, sizes, ranges);

//            int histW = 640; int histH = 480;
//            cv::Mat histImage(histH, histW, CV_8UC3, cv::Scalar(255,255,255));

//            cv::normalize(histPlanes[0], histPlanes[0], 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
//            cv::normalize(histPlanes[1], histPlanes[1], 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
//            cv::normalize(histPlanes[2], histPlanes[2], 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

//            /// Draw for each channel
//            for(int i = redThreshold[0]; i <= redThreshold[1]; ++i)
//                cv::line(histImage, cv::Point(histW*(i-1)/256.0, histH - histPlanes[0].at<float>(i-1)),
//                      cv::Point(histW*i/256.0, histH - histPlanes[0].at<float>(i)),
//                      cv::Scalar(255, 0, 0), 1, 8, 0);
//            for(int i = greenThreshold[0]; i <= greenThreshold[1]; ++i)
//                cv::line(histImage, cv::Point(histW*(i-1)/256.0, histH - histPlanes[1].at<float>(i-1)),
//                      cv::Point(histW*i/256.0, histH - histPlanes[1].at<float>(i)),
//                      cv::Scalar(0, 255, 0), 1, 8, 0);
//            for(int i = blueThreshold[0]; i <= blueThreshold[1]; ++i)
//                cv::line(histImage, cv::Point(histW*(i-1)/256.0, histH - histPlanes[2].at<float>(i-1)),
//                      cv::Point(histW*i/256.0, histH - histPlanes[2].at<float>(i)),
//                      cv::Scalar(0, 0, 255), 1, 8, 0);

//            cv::Mat threshMask;
//            cv::Mat thresholdedframe;
//            cv::inRange(frame,
//                        cv::Scalar(redThreshold[0], greenThreshold[0], blueThreshold[0]),
//                        cv::Scalar(redThreshold[1], greenThreshold[1], blueThreshold[1]),
//                        threshMask);
//            frame.copyTo(thresholdedframe, threshMask);

//            cv::Canny(framePlanes[0], edgePlanes[0], redThreshold[0], redThreshold[1]);
//            cv::Canny(framePlanes[1], edgePlanes[1], greenThreshold[0], greenThreshold[1]);
//            cv::Canny(framePlanes[2], edgePlanes[2], blueThreshold[0], blueThreshold[1]);
//            edges = edgePlanes[0] & edgePlanes[1] & edgePlanes[2];

//            cv::morphologyEx(edges, edges, cv::MORPH_CLOSE, cv::noArray(), cv::Point(-1,-1), 5);

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
            cv::drawContours(frame, contours, -1, cv::Scalar(0,255,0), CV_FILLED, 8);

            cv::imshow("Video Input", frame);
            if(!contours.empty())
            {
                std::cout << "Area = " << cv::contourArea(contours[0])<<std::endl;
            }
            cv::imshow("Input Histogram", edges);
        }
        if(cv::waitKey(30) >= 0) break;
    }
    return 0;
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
