#include <iostream>
#include "opencv2/opencv.hpp"
// using namespace std;

int main() {

    cv::VideoCapture webcam(0);

    if ( !webcam.isOpened() ) {
        std::cerr << "Camera pepsi" << std::endl;
        return 1; 
    }


    cv::namedWindow("window", cv::WINDOW_AUTOSIZE);

    cv::Mat frame;

    webcam >> frame;

    while(1) {
        cv::imshow("sexkamerka", frame);

        if (cv::waitKey(10) >= 0)
            break;
    }
    return 0;
}