#include <iostream>
#include <chrono>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#define COLOR_MODE 0

using Duration = std::chrono::milliseconds;

enum COLORS {ORANGE, GREEN, BLUE};



int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

struct gpreset {
    int H_MIN = 42;
    int H_MAX = 123;
    int S_MIN = 42;
    int S_MAX = 185;
    int V_MIN = 93;
    int V_MAX = 205;
} gpreset;

struct orangepreset {
    int H_MIN = 3;
    int H_MAX = 194;
    int S_MIN = 98;
    int S_MAX = 218;
    int V_MIN = 200;
    int V_MAX = 256;
} orangepreset;

int MIN_BALL_AREA = 10 * 10;

std::string TRACKBAR_WINDOW = "Suwaki";

void setMode() {

    if (COLOR_MODE == ORANGE) {
        H_MIN = orangepreset.H_MIN;
        H_MAX = orangepreset.H_MAX;
        S_MIN = orangepreset.S_MIN;
        S_MAX = orangepreset.S_MAX;
        V_MIN = orangepreset.V_MIN;
        V_MAX = orangepreset.V_MAX;
    } 
    else if ( COLOR_MODE == GREEN) {
        H_MIN = gpreset.H_MIN;
        H_MAX = gpreset.H_MAX;
        S_MIN = gpreset.S_MIN;
        S_MAX = gpreset.S_MAX;
        V_MIN = gpreset.V_MIN;
        V_MAX = gpreset.V_MAX;
    }
}


void trackbarValChange( int, void* ) {}

void initTrackbars() {

    cv::namedWindow(TRACKBAR_WINDOW, 0);
    	
    char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);

    cv::createTrackbar( "H_MIN", TRACKBAR_WINDOW, &H_MIN, H_MAX, trackbarValChange );
    cv::createTrackbar( "H_MAX", TRACKBAR_WINDOW, &H_MAX, H_MAX, trackbarValChange );
    cv::createTrackbar( "S_MIN", TRACKBAR_WINDOW, &S_MIN, S_MAX, trackbarValChange );
    cv::createTrackbar( "S_MAX", TRACKBAR_WINDOW, &S_MAX, S_MAX, trackbarValChange );
    cv::createTrackbar( "V_MIN", TRACKBAR_WINDOW, &V_MIN, V_MAX, trackbarValChange );
    cv::createTrackbar( "V_MAX", TRACKBAR_WINDOW, &V_MAX, V_MAX, trackbarValChange );

}



int main() {
    
    cv::Mat frame;
    cv::Mat frameHSV;
    cv::Mat frameThreshold;
    cv::VideoCapture webcam (0);

    setMode();
    //initTrackbars();

    if ( !webcam.isOpened() ) {
        std::cout << "camera isded" << std::endl;
        return 1;
    }

    std::vector< std::vector< cv::Point >> contours;
    std::vector< cv::Vec4i > hierarchy;
    std::vector< std::chrono::duration<double, std::milli> > filter_time;
    std::chrono::duration<double, std::milli> total_time;

    int licznik = 0;
    

    auto start = std::chrono::steady_clock::now();

    while (true) {

        webcam.read(frame);
        licznik++;

        auto filter_start = std::chrono::high_resolution_clock::now();
        cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);
    
        cv::inRange(frameHSV, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), frameThreshold);

        cv::findContours(frameThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
        // std::sort(contours.begin(), contours.end(), cv::contourArea());


        for( int i = 0; i < contours.size(); i++ ) 
        {
            if ( cv::contourArea(contours.at(i) ) > MIN_BALL_AREA )
            {

                cv::drawMarker(frame, contours.at(i).at(0), cv::Scalar(0,0,255));
                break;
            }
        }        


        auto filter_end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> fp_ms = filter_end - filter_start;

        filter_time.push_back(fp_ms);


        cv::imshow("Kamerka", frame);
        cv::imshow("Filtr", frameThreshold);

        if ( cv::waitKey(1) >= 0 )
            break;
    
    }

    

    //count total time in ms
    for (int i = 0; i < filter_time.size(); i++)
    {
        total_time += filter_time.at(i);
        std::cout<< filter_time.at(i).count() << std::endl;
    }


    
    auto avg_time = total_time / filter_time.size();
    std::cout << avg_time.count() << std::endl;



    // auto end = std::chrono::steady_clock::now();
    // std::chrono::duration<double> seconds = end - start;

    // double klatki = licznik / seconds.count();
    // std::cout << klatki << " FPS" << std::endl;


    return 0;
}