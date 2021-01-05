
#include "../include/producerprocess.hpp"
ProducerProcess::ProducerProcess() {
    loadInstantFrames();
}
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
    //to-do urgent no kurna wez to zmien czlowieku to nie jest rownolegle, potencjalny lock
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
        // readFrame();
        getRequiredMove();
        tryChoosingFrame();
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

void ProducerProcess::getRequiredMove() {
    shque.pop(&message_in);

    std::cout << "[P] Received Message with key: " << message_in.key << std::endl;
}


void ProducerProcess::tryChoosingFrame() {

    if(message_in.key == 'w')
        temp_frame = move_jump.clone();
    else if (message_in.key == 's')
        temp_frame = move_duck.clone();
    else
        temp_frame = move_idle.clone();

    frameBuffer.push_back(temp_frame);
    frame_counter++;
    std::cout << "\n-----------------" << frame_counter << "-----------------\n" << std::flush;
}

void ProducerProcess::loadInstantFrames() {
    move_jump = cv::imread("../assets/klatka-gora.jpg", 0);
    move_idle = cv::imread("../assets/klatka-non.jpg", 0);
    move_duck = cv::imread("../assets/klatka-dol.jpg", 0);

    if (move_jump.empty() || move_idle.empty() || move_duck.empty())
        std::cout << "[!!!] Failed reading imgs!" << std::endl;
}