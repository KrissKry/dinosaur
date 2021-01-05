#include "../include/gameprocess.hpp"



GameProcess::GameProcess() {
    
    current_deadline = std::chrono::system_clock::now();
    // this->max_waiting_time = max_waiting_time;
    max_waiting_time = 10;

    game_action = sem_open(GAME_SEM, O_CREAT, 0644, 0);

    // std::thread runn_bitch(&GameProcess::run, this);
    // runn_bitch.join();
}

GameProcess::~GameProcess() {
    // delete shque;s
}

void GameProcess::generateNextMove() {

    if (!failed) {
        std::cout << "[G] Generating next move." << std::endl;
        current_key = rg.generateMove();
        std::chrono::system_clock::time_point x = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point xd = x + std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::duration< double >(max_waiting_time) );
        // current_deadline = std::chrono::system_clock::now() + std::chrono::duration<double, std::milli>(max_waiting_time);
        // current_deadline = x + std::chrono::minutes(max_waiting_time);
        // std::chrono::system_clock::time_point xd = x + std::chrono::milliseconds(max_waiting_time);
        
        // timepoint_t x = std::chrono::system_clock::now();

        message_out.key = current_key;
        message_out.deadline = xd;
        std::cout << "[G] Next move: " << current_key << " with deadline " << std::endl;
    }
    else {
        std::cout << "[G] Not generating next move." << std::endl;
        // sem_wait(game_action);
        usleep(100000000);
    }
}


void GameProcess::sendPing() {
    // shque->push
    if (!failed)
        shque.push(&message_out);

}

// void GameProcess::receiveMove(char key) {

    
//     message_in.y = key;
//     message_in.deadline = std::chrono::system_clock::now();
//     std::cout << "[G] Received key " << key << "@";// << message_in.deadline << std::endl;
//     //continue game running
//     sem_post(game_action);
// }

void GameProcess::run() {


    do {
        std::cout << "[G] Game run loop\n";
        generateNextMove();
        sendPing();
        sem_wait(game_action);

    } while (std::chrono::duration<double, std::milli>(message_in.deadline - message_out.deadline) < std::chrono::duration<double, std::milli>(0) );
    std::cout << "ended lmao\n";

    // return true;
}



[[noreturn]] void GameProcess::bitchImRunning() {


    while (true) {
        std::cout << "[G] Game running in the 90s" << std::endl;
        
        generateNextMove();
        sendPing();
        waitForSignal();
        validateSignal();
        
    }
}


void GameProcess::waitForSignal() {

    steeringQueue.pop(&message_in);
}



void GameProcess::validateSignal() {

    if (message_in.key == message_out.key &&
        std::chrono::duration<double, std::milli>(message_in.deadline - message_out.deadline)  <  std::chrono::duration<double, std::milli>(0)) 

        std::cout << "[G] Successful move." << std::endl;
    
    else {
        std::cout << "[G] Game is fked now." << std::endl;
        failed = true;
    }
}