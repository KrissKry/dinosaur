#include "../include/producerprocess.hpp"

ProducerProcess::ProducerProcess() {

    if (TEST_MODE)
        loadInstantFrames();
}


[[noreturn]] void ProducerProcess::run() {
    
    std::cout << "[I] Producer running [[noreturn]].\n";

    while ( true ) {

        getRequiredMove();

        if (!TEST_MODE )
            readFrame();
        else
            tryChoosingFrame();

        convertFrame();
        sendFrame();

    }
}


void ProducerProcess::readFrame() {

    webcam.read(temp_frame);
    frameBuffer.push_back(temp_frame);
    frame_counter++;


    // std::cout << "\n-----------------" << frame_counter << "-----------------\n";
    // cv::imshow("Oryginalna", temp_frame);
    // cv::waitKey(0);
}

void ProducerProcess::convertFrame() {
    
    cv::Mat frame = frameBuffer.front();
    frameBuffer.pop_front();

    //convert cv::Mat to bytes
    memcpy(output_buffer, frame.data, FRAME_SIZE );

}

void ProducerProcess::sendFrame() {

    if (CNSL_LOG)
        std::cout << "[P] Sending: " << output_buffer[0] << output_buffer[1] << output_buffer[2] << std::endl;

    //use shared queue and shared memory to put current frame in the buffer
    membuf.push(FRAME_SIZE, output_buffer);

}




void ProducerProcess::getRequiredMove() {

    shque.pop(&message_in);



    if (CNSL_LOG || (!TEST_MODE && current_deadline != message_in.deadline))
        std::cout << "[P] Game is asking for '" << message_in.key << "' key." << std::endl;

    if( current_deadline != message_in.deadline )
        current_deadline = message_in.deadline;
}


void ProducerProcess::tryChoosingFrame() {

    if(message_in.key == 'w') {

        temp_frame = move_jump.clone();
        if (CNSL_LOG)
            std::cout << "[P] Cloned jump frame" << std::endl << std::flush;
    
    } else if (message_in.key == 's') {

        temp_frame = move_duck.clone();
        if (CNSL_LOG)
            std::cout << "[P] Cloned duck frame" << std::endl << std::flush;
    
    } else {

        temp_frame = move_idle.clone();
        if (CNSL_LOG)
            std::cout << "[P] Cloned idle frame" << std::endl << std::flush;

    }

    frameBuffer.push_back(temp_frame);
    frame_counter++;
    
    if (CNSL_LOG)
        std::cout << "\n-----------------" << frame_counter << "-----------------\n" << std::flush;
}


void ProducerProcess::loadInstantFrames() {

    move_jump = cv::imread("../assets/klatka-gora.png", cv::ImreadModes::IMREAD_COLOR);
    move_idle = cv::imread("../assets/klatka-non.png", cv::ImreadModes::IMREAD_COLOR);
    move_duck = cv::imread("../assets/klatka-dol.png", cv::ImreadModes::IMREAD_COLOR);

    if (move_jump.empty() || move_idle.empty() || move_duck.empty())
        std::cout << "[!!!] Failed reading imgs!" << std::endl;

}