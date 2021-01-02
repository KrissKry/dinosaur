
#include "../include/producerprocess.hpp"

void ProducerProcess::readFrame() {
    //well obviously read a fucking frame ay
    cv::Mat frame;
    webcam.read(frame);
    frameBuffer.push_back(frame);
    frame_counter++;
}

void ProducerProcess::convertFrame() {
    
    //add semaphore await? or sth
    cv::Mat frame;
    // std::vector<std::byte< v_char //needs c++17
    std::vector<unsigned char> v_char;
    
    do {
        if ( frameBuffer.size() > 0) {
            frame = frameBuffer.front();
            break;
        }
    
    } while ( frameBuffer.size() == 0);
    
    //convert cv::Mat to bytes
    for (unsigned int y = 0; y < frame.rows; y++)
    {
        for(unsigned int x = 0; x < frame.cols; x++)
        {
            v_char.push_back(*(uchar*)(frame.data + y * frame.step + x));
        }
    }
    //these converted bytes add to char buffer
    // shque.push
    // shmem.coś
}

void ProducerProcess::sendFrame() {
    //use shared queue and shared memory to put current frame in the buffer
    
}

[[noreturn]] void ProducerProcess::run() {

    // resize singleframebuf in bytes properly;

    webcam.open(0);
    // shque = true;
    

    if ( !webcam.isOpened() ) {
        // ??
        std::cerr << "bruh" << std::endl;
        return;
    }
    //tutaj dodac zegar
    while (true) {
        readFrame();
        convertFrame();
        sendFrame();
    }

    // tutaj dodac drugi zegar
    // policzyc fps x d
    std::cout << "Frames read: " << frame_counter << std::endl;
    // std::cout << "FPS: " << tutaj << std::endl;
}