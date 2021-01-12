#ifndef PRODUCERPROCESS_HPP
#define PRODUCERPROCESS_HPP

#include <deque>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "messagequeue.hpp"
#include "memorybuffer.hpp"

/*
    Producer reads a frame from webcam and converts it to bytes to send it in a shared memory.
    After putting it in a buffer, it sends a com. 
*/
class ProducerProcess {


    private:
        // SharedQueue shque = true;
        MemoryBuffer membuf = true;
        MessageQueue shque = {false, MQ_PRODUCER_GAME};
        game_message message_in;
        

        cv::VideoCapture webcam = cv::VideoCapture(0);

        std::deque<cv::Mat> frameBuffer;
        
        std::chrono::system_clock::time_point current_deadline{};
        cv::Mat temp_frame;
        cv::Mat frame;
        cv::Mat move_duck;
        cv::Mat move_jump;
        cv::Mat move_idle;
        
        int frame_counter{}; 
        
        char* output_buffer = new char[FRAME_SIZE];


    public:
        ProducerProcess();
        ~ProducerProcess() {
            delete output_buffer;
        }

        void readFrame();
        void convertFrame();
        void sendFrame();
        // void TestOnce();
        void getRequiredMove();
        void tryChoosingFrame();
        void loadInstantFrames();
        [[noreturn]] void run();
};

#endif