#include "../include/controlprocess.hpp"


// ControlProcess::ControlProcess() {
//     // shque = new SharedQueue(false, MQ_FILTER_CLIENT);
//     // shque = MessageQueue(false, MQ_FILTER_CLIENT);
// }
ControlProcess::ControlProcess() {
    // GameProcess* game;
    // game_thread = std::thread(game.run);
    // game_thread.join();
}

void ControlProcess::readCoords() {
    
    shque.pop(&current_coords);
}

void ControlProcess::send_steering_signal() {

    
    if (current_coords.y > Y_REG)
        pressed_key = 's';
    else if ( current_coords.y == Y_REG)
        pressed_key = ' ';
    else
        pressed_key = 'w';

    current_move.key = pressed_key;
    current_move.deadline = std::chrono::system_clock::now();
    // game.receiveMove(pressed_key);
    if (CNSL_LOG)
        std::cout << "[C] Key pressed '" << current_move.key << "'" << std::endl << std::flush;
    game_queue.push(&current_move);
}


[[noreturn]] void ControlProcess::run() {

    std::cout << "[I] Controller running [[noreturn]]." << std::endl;
    // std::thread runn_bitch(&game.run);
    // runn_bitch.join();
    usleep(500000);
    while (true) {
        readCoords();
        if (CNSL_LOG)
            std::cout << "[C] Read coords " << current_coords.x << " " << current_coords.y << std::endl; 
        send_steering_signal();
        // usleep(1000000);
    }
}