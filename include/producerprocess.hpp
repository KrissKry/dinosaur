#ifndef PRODUCERPROCESS_HPP
#define PRODUCERPROCESS_HPP

#include <deque>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "sharedqueue.hpp"
#include "memorybuffer.hpp"

/*
    Producer reads a frame from webcam and converts it to bytes to send it in a shared memory.
    After putting it in a buffer, it sends a com. 
*/
class ProducerProcess {


    private:
        // SharedQueue shque = true;

        MemoryBuffer membuf = true;

        cv::VideoCapture webcam = cv::VideoCapture(0);

        std::deque<cv::Mat> frameBuffer;
        
        cv::Mat temp_frame;
        cv::Mat frame;
        
        int frame_counter{}; 
        
        char* output_buffer = new char[WIDTH*HEIGHT*3];

    public:
        ProducerProcess() {}
        ~ProducerProcess() {}

        void readFrame();
        void convertFrame();
        void sendFrame();
        // void TestOnce();

        [[noreturn]] void run();
};

#endif