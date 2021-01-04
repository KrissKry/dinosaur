#include "../include/filterprocess.hpp"


[[noreturn]] void FilterProcess::run() {
    std::cout << "buenos dias\n";
    // std::cout<< "Filtering awoken\n";
    while (true) {
        // testFilter();
        // getFrame();
        // convertFrame();
        // handleFrame();
        // sendCoords();
    }
}
void FilterProcess::TestOnce() {
    // data test_data;
    char *buforkurwa = new char [WIDTH*HEIGHT*3];
    std::cout << "Trying popin\n";
    membuf.pop(sizeof(buforkurwa), buforkurwa);

    // shmem.pop(buforkurwa);
    std::cout << buforkurwa[0] << buforkurwa[1] << buforkurwa[2] << std::endl;
}
// void FilterProcess::getFrame() {
//     // memcpy(frame_bytes, shmem.pop(), )
// }

// void FilterProcess::convertFrame() {
//     unsigned char* buffer[WIDTH*HEIGHT*3]{};

//     // shmem(tu dzikie argumenty)
//     // memcpy(buffer, shmem.pop(), WIDTH*HEIGHT*3);
//     // shmem.pop(buffer);
//     // buffer = shmem.pop();
//     // std::vector<unsigned char> byte_frame(buffer, buffer + WIDTH*HEIGHT*3);
//     // frame = cv::Mat()
//     // frame = cv::Mat()
//     // memcpy()
// }
// void FilterProcess::handleFrame() {
//     // auto filter_start = std::chrono::high_resolution_clock::now();
//     cv::Mat frameHSV;
//     cv::Mat frameThreshold;
//     std::vector< std::vector< cv::Point >> contours;
//     std::vector< cv::Vec4i > hierarchy;

//     cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);
    
//     cv::inRange(frameHSV, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), frameThreshold);

//     cv::findContours(frameThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

//     // moznaby zaimplementowac sortowanie - porowanine czasu przetwarzania potrzebne :>
//     // std::sort(contours.begin(), contours.end(), cv::contourArea());


//     for( int i = 0; i < contours.size(); i++ ) 
//     {
//         if ( cv::contourArea(contours.at(i) ) > MIN_BALL_AREA )
//         {

//             // cv::drawMarker(frame, contours.at(i).at(0), cv::Scalar(0,0,255));
//             coords.x = contours.at(i).at(0).x;
//             coords.y = contours.at(i).at(0).y;
//             coords.timestamp = std::chrono::system_clock::now();
//             break;
//         }
//     }        
// }

// void FilterProcess::sendCoords() {

//     shque.push(&coords);
// }

