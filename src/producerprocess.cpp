
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
    // std::vector<uchar> v_char;
    // char buf[WIDTH*HEIGHT];
    do {
        if ( frameBuffer.size() > 0) {
            frame = frameBuffer.front();
            frameBuffer.pop_front();
            break;
        }
    
    } while ( frameBuffer.size() == 0);
    
    //convert cv::Mat to bytes
    int size = frame.total() * frame.elemSize();
    std::cout<< "size got: " << size << std::endl;
    // unsigned char * buf = new unsigned char[size];
    // std::cout << buf[0] << " ";
    unsigned char *buf = new unsigned char[WIDTH * HEIGHT * 3];
    memcpy(buf, frame.data, WIDTH*HEIGHT*3 );
    std::cout << "copied shit\n" << std::fflush;
    // std::cout << buf[0] << " ";
    // for (unsigned int y = 0; y < frame.rows; y++)
    // {
    //     for(unsigned int x = 0; x < frame.cols; x++)
    //     {
    //         v_char.push_back(*(uchar*)(frame.data + y * frame.step + x));

    //     }
    // }

    // std::cout<< "liczba elementow obrazka: " << v_char.size() << std::endl;
    //these converted bytes add to char buffer
    // shque.push
    // shmem.coś

    shmem.push(buf);    
    std::cout << "sent to shmem\n" << std::fflush;
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
        std::cerr << "Webcam failed to open" << std::endl;
        
    } else {
        std::cout<< "Webcam opened" << std::endl;
    }

    //tutaj dodac zegar
    int cnt = 0;
    while ( true ) {
    // while ( webcam.isOpened() ) {
        readFrame();
        convertFrame();
        // sendFrame();
        std::cout << "iteration no. " << cnt << std::fflush;
        cnt++;
    }

    // tutaj dodac drugi zegar
    // policzyc fps x d
    // std::cout << "FPS: " << tutaj << std::endl;
}