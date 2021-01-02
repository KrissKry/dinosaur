#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/ximgproc/segmentation.hpp"
#include "opencv2/highgui.hpp"
// using namespace std;
#define MIN_RADIUS 10
#define RED_LOW 155
#define RED_HIGH 200
#define MODE 0


class Ball {
    public:

        Ball();
        
        Ball(cv::Rect rect, int x, int y);


        // void getBall();
    private:
        cv::Point position;
        cv::Rect rect;
};


Ball::Ball() {}

Ball::Ball(cv::Rect rect, int x, int y) 
{

    this->rect = rect;
    this->position = cv::Point(x, y);
}


cv::Scalar ballHigh = cv::Scalar();
cv::Scalar ballLow = cv::Scalar();
int main() {

    cv::VideoCapture webcam(0);

    if ( !webcam.isOpened() ) {
        std::cerr << "Camera pepsi" << std::endl;
        return 1; 
    }
    cv::setUseOptimized(true);
    cv::setNumThreads(4);

    //cv::namedWindow("window", cv::WINDOW_AUTOSIZE);

    cv::Mat frame;
    cv::Mat frameHSV;
    cv::Mat frameThreshold;  

    int objLowH = 172;
    int objLowS = 62;
    int objLowV = 70;
    int objHighH = 243;
    int objHighS = 95;
    int objHighV = 90;

    bool captured;
    // webcam >> frame;
    // cv::resize(frame, frame, cv::Size(800, 800 * 720 / 1280));
    

    // cv::Mat frameOut;
    // cv::Ptr<cv::ximgproc::segmentation::SelectiveSearchSegmentation> ss = cv::ximgproc::segmentation::createSelectiveSearchSegmentation();
    // ss->setBaseImage(frame);
    // ss->switchToSelectiveSearchFast();
    // std::vector<cv::Rect> rects;
    // ss->process(rects);
    // std::cout << "Rects count: "<< rects.size() << std::endl;
    // frameOut = frame.clone();
    
    // for(int i = 0; i < rects.size(); i++) {

    //     if (i < 10)
    //         std::cout << rects.at(i) << std::endl;
    //     if (i < 100) {
    //         cv::rectangle(frameOut, rects[i], cv::Scalar(255, 0, 0));


    //     }
    //     else {
    //         break;
    //     }

    // }

    // cv::imshow("klatka", frame);
    // cv::imshow("klatka po", frameOut);
    // int k = cv::waitKey();  
    // while (true) {
    //     captured = webcam.read(frame);

    //     cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);

    //     cv::inRange(frameHSV, cv::Scalar(objLowH, objLowS, objLowV), cv::Scalar(objHighH, objHighS, objHighV), frameThreshold);

    //     // cv::bitwise_and()
    //     cv::erode(frameThreshold, frameThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    //     cv::dilate(frameThreshold, frameThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    //     cv::dilate(frameThreshold, frameThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    //     cv::erode(frameThreshold, frameThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    //     cv::imshow("Thresholded", frameThreshold);
    //     cv::imshow("Original", frame);


    //     if ( cv::waitKey(20) >= 0) 
    //         break;
    // }
    return 0;
}