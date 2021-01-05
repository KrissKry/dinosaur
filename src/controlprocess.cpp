#include "../include/controlprocess.hpp"


// ControlProcess::ControlProcess() {
//     // shque = new SharedQueue(false, MQ_FILTER_CLIENT);
//     // shque = MessageQueue(false, MQ_FILTER_CLIENT);
// }


void ControlProcess::readCoords() {
    
    shque.pop(&current_coords);
}

void ControlProcess::send_steering_signal() {

    
    if (current_coords.y > Y_REG)
        pressed_key = 'w';
    else if ( current_coords.y == Y_REG)
        pressed_key = ' ';
    else
        pressed_key = 's';

    // game.receiveMove(pressed_key);
}


[[noreturn]] void ControlProcess::run() {

    std::cout << "[I] Controller running [[noreturn]]." << std::endl;
    
    while (true) {
        readCoords();
        std::cout << "[C] Read coords " << current_coords.x << " " << current_coords.y << std::endl; 
        send_steering_signal();
        usleep(1000000);
    }
}