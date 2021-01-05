
#include "../include/producerprocess.hpp"

void ProducerProcess::readFrame() {

    // cv::Mat temp_frame;
    webcam.read(temp_frame);
    frameBuffer.push_back(temp_frame);
    frame_counter++;
    std::cout << "\n-----------------" << frame_counter << "-----------------\n";
    // cv::imshow("Oryginalna", temp_frame);
    // cv::waitKey(0);
}

void ProducerProcess::convertFrame() {
    
    //to-do to chyba do zmiany jednak 
    do {
        if ( frameBuffer.size() > 0) {

            frame = frameBuffer.front();
            frameBuffer.pop_front();
            break;
        }
    
    } while ( frameBuffer.size() == 0);
    
    
    // int size = frame.total() * frame.elemSize();
    // std::cout << "Rozmiar klatki mat: " << size << std::endl;
    // std::cout << "Rozmiar bufora:" << sizeof(output_buffer)<< std::endl;
    // std::cout << "Rozmiar wsk bufora" << sizeof(*output_buffer) << std::endl;
    
    //convert cv::Mat to bytes
    memcpy(output_buffer, frame.data, WIDTH*HEIGHT*3 );



}

void ProducerProcess::sendFrame() {

    //use shared queue and shared memory to put current frame in the buffer
    std::cout << "[P] Sending: " << output_buffer[0] << output_buffer[1] << output_buffer[2] << std::endl;
    // std::cout << "aa: " << output_buffer[200000] << " " << output_buffer[2000000] << std::endl;
    membuf.push(FRAME_SIZE, output_buffer);

}

[[noreturn]] void ProducerProcess::run() {
    
    std::cout << "[I] Producer running [[noreturn]].\n";

    while ( true ) {
        readFrame();
        convertFrame();
        sendFrame();
        
        sleep(1);
    }

    // tutaj dodac drugi zegar
    // policzyc fps x d
    // std::cout << "FPS: " << tutaj << std::endl;
}

// void ProducerProcess::TestOnce()
// {
//     readFrame();
//     convertFrame();
//     sendFrame();
// }