#ifndef PRODUCERPROCESS_HPP
#define PRODUCERPROCESS_HPP

#include <deque>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "sharedmemory.hpp"
#include "sharedqueue.hpp"
#include <vector>

/*
    Producer reads a frame from webcam and converts it to bytes to send it in a shared memory.
    After putting it in a buffer, it sends a com. 
*/
class ProducerProcess {


    private:
        // SharedQueue shque = true;
        // SharedMemory shmem = new SharedMemory(true);
        SharedMemory shmem = true;

        cv::VideoCapture webcam;
        std::deque<cv::Mat> frameBuffer;
        int frame_counter{}; 
        char* convertedSingleFrame;

    public:
        ProducerProcess() {}
        ~ProducerProcess() {}

        void readFrame();
        void convertFrame();
        void sendFrame();
        
        [[noreturn]] void run();
};

#endif