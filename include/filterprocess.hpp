#ifndef FILTERPROCESS_HPP
#define FILTERPROCESS_HPP
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "sharedqueue.hpp"
#include "sharedmemory.hpp"
#include "util.hpp"
#include "memorybuffer.hpp"
/*
    FilterProcess awaits for a message in shared memory and then reads frame from shared memory.
    It has to convert the frame from bytes to opencv::mat type. 
*/


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
        // SharedQueue shque;
        // SharedMemory shmem = false;
        MemoryBuffer membuf = false;

        cv::Mat frame;
        unsigned char *frame_bytes;

        coords_message coords{};

    public:
        FilterProcess() {}
        ~FilterProcess() {}
        void TestOnce();
        // void getFrame();
        // void convertFrame();
        // void handleFrame();

        // void sendCoords();

        [[noreturn]] void run();



};



#endif