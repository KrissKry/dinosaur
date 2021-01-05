#ifndef FILTERPROCESS_HPP
#define FILTERPROCESS_HPP
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

// #include "sharedqueue.hpp"
#include "messagequeue.hpp"
#include "util.hpp"
#include "memorybuffer.hpp"

/*
    1. Acquire a frame from shared memory
    2. Convert it to coords
    3. Send the coords in shared queue

*/
//orange preset 
#define H_MIN 3
#define H_MAX 194
#define S_MIN 98
#define S_MAX 218
#define V_MIN 200
#define V_MAX 256
#define MIN_BALL_AREA 10*10

class FilterProcess {

    private:
        // SharedQueue* shque;
        
        MemoryBuffer membuf = false;
        MessageQueue shque = { true, MQ_FILTER_CLIENT };
       
        cv::Mat frame;
        char *frame_bytes = new char[WIDTH*HEIGHT*3]; 

        coords_message coords;

    public:
        FilterProcess() {
            // shque = new SharedQueue(true, MQ_FILTER_CLIENT);
        }
        ~FilterProcess() {
            // delete shque;
        }
        // void TestOnce();

        void getFrame();
        void convertFrame();
        void handleFrame();

        void sendCoords(int cnt);

        [[noreturn]] void run();



};



#endif