#include "../include/filterprocess.hpp"


[[noreturn]] void FilterProcess::run() {
    
    usleep(500000);
    std::cout << "[I] Filter running [[noreturn]].\n";

    while (true) {

        getFrame();
        convertFrame();
        handleFrame();
        sendCoords();

    }
}


void FilterProcess::getFrame() {
    
    membuf.pop(FRAME_SIZE, frame_bytes);
    if (CNSL_LOG)
        std::cout << "[F] Received: " << frame_bytes[0] << frame_bytes[1] << frame_bytes[2] << std::endl;
}



void FilterProcess::convertFrame() {

    frame = cv::Mat(HEIGHT, WIDTH, CV_8UC3, &frame_bytes[0]);
}



void FilterProcess::handleFrame() {

    cv::Mat frameHSV;
    cv::Mat frameThreshold;
    std::vector< std::vector< cv::Point >> contours;
    std::vector< cv::Vec4i > hierarchy;

    cv::cvtColor(frame, frameHSV, cv::COLOR_RGB2HSV);
    
    cv::inRange(frameHSV, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), frameThreshold);

    cv::findContours(frameThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    // moznaby zaimplementowac sortowanie - porowanine czasu przetwarzania potrzebne :>
    // std::sort(contours.begin(), contours.end(), cv::contourArea());
    // cv::imshow("Kamerka", frame);
    // cv::imshow("Filtr", frameThreshold);

        // if ( cv::waitKey(1) >= 0 )
            // std::cout << "xdd";

    // cv::imshow("oryg", frame);
    // cv::imshow("hsv", frameHSV);
    // cv::imshow("konwert", frameThreshold);
    // cv::waitKey(0);

    if (CNSL_LOG)
        std::cout << "[F] Looking for decent contours." << std::endl << std::flush;


    for( int i = 0; i < contours.size(); i++ ) 
    {
        if ( cv::contourArea(contours.at(i) ) > MIN_BALL_AREA )
        {

            coords.x = contours.at(i).at(0).x;
            coords.y = contours.at(i).at(0).y;
            coords.timestamp = std::chrono::system_clock::now();
            if (CNSL_LOG)
                std::cout << "[F] Found contours at: " << coords.x << " " << coords.y << std::endl << std::flush;
            return;
        }
    }        
    //in case of object not found, idle move
    coords.y = Y_REG;
}

void FilterProcess::sendCoords() {

    if (CNSL_LOG)
        std::cout << "[F] Sending coords " << coords.x << " " << coords.y << std::endl;
        
    shque.push(&coords);
}

