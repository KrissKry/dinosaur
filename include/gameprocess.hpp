#ifndef SCZR_GAMEPROCESS_H
#define SCZR_GAMEPROCESS_H

#include <deque>
#include <chrono>
#include <iostream>
#include <thread>
#include <semaphore.h>
#include "util.hpp"
#include "messagequeue.hpp"
#include "randomgenerator.hpp"

// typedef std::chrono::time_point< std::chrono::system_clock, double_prec_seconds > timepoint_t;

class GameProcess {

    private:
        MessageQueue shque = {true, MQ_PRODUCER_GAME};
        MessageQueue steeringQueue = {false, MQ_CLIENT_GAME};

        RandomGenerator rg;
        std::deque<std::pair <enum Key_Press, std::chrono::system_clock::time_point> > movements{};
        
        sem_t* game_action;
        bool failed = false;
        // double time_scaling_factor{};
        double max_waiting_time{};
        std::chrono::system_clock::time_point current_deadline;
        
        game_message message_out; 
        game_message message_in;
        char current_key{};

    public:
        GameProcess();
        ~GameProcess();

        void waitForSignal();
        void validateSignal();
        void generateNextMove();
        void sendPing();


        void receiveMove(char button) {}
        [[noreturn]] void run();

        // static void* gameRunner(void* context);
        // [[noreturn]] void bitchImRunning();
};

#endif