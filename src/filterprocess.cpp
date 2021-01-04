#include "../include/filterprocess.hpp"


[[noreturn]] void FilterProcess::run() {
    std::cout << "[I] Filter running [[noreturn]].\n";

    while (true) {
        // TestOnce();
        // testFilter();
        getFrame();
        convertFrame();
        // handleFrame();
        // sendCoords();
        sleep(1);
    }
}
// void FilterProcess::TestOnce() {

//     char *buforkurwa = new char [WIDTH*HEIGHT*3];
//     // std::cout << "Trying popin\n";
//     membuf.pop(BUFFER_SIZE, buforkurwa);

//     // shmem.pop(buforkurwa);
//     // std::cout << "otrzymano: " << buforkurwa[0] << buforkurwa[1] << buforkurwa[2] << std::endl;
//     // std::cout << "aa: " << buforkurwa[200000] << " " << buforkurwa[2000000] << std::endl;
    
//     cv::Mat frame(720, 1280, CV_8UC3, &buforkurwa[0]);

//     cv::imshow("otrzymane", frame);
//     cv::waitKey(0);

// }

void FilterProcess::getFrame() {
    
    membuf.pop(BUFFER_SIZE, frame_bytes);
    std::cout << "[F] Received: " << frame_bytes[0] << frame_bytes[1] << frame_bytes[2] << std::endl;
    // std::cout << "dalej: " << frame_bytes[200000] << " " << frame_bytes[2000000] << std::endl;
}



void FilterProcess::convertFrame() {

    frame = cv::Mat(HEIGHT, WIDTH, CV_8UC3, &frame_bytes[0]);
}



void FilterProcess::handleFrame() {
    // auto filter_start = std::chrono::high_resolution_clock::now();
    cv::Mat frameHSV;
    cv::Mat frameThreshold;
    std::vector< std::vector< cv::Point >> contours;
    std::vector< cv::Vec4i > hierarchy;

    cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);
    
    cv::inRange(frameHSV, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), frameThreshold);

    cv::findContours(frameThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    // moznaby zaimplementowac sortowanie - porowanine czasu przetwarzania potrzebne :>
    // std::sort(contours.begin(), contours.end(), cv::contourArea());


    for( int i = 0; i < contours.size(); i++ ) 
    {
        if ( cv::contourArea(contours.at(i) ) > MIN_BALL_AREA )
        {

            // cv::drawMarker(frame, contours.at(i).at(0), cv::Scalar(0,0,255));
            coords.x = contours.at(i).at(0).x;
            coords.y = contours.at(i).at(0).y;
            coords.timestamp = std::chrono::system_clock::now();
            break;
        }
    }        
}

// void FilterProcess::sendCoords() {

//     shque.push(&coords);
// }
