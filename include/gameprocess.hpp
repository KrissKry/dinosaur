#ifndef SCZR_GAMEPROCESS_H
#define SCZR_GAMEPROCESS_H

#include <deque>
// #include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
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

        std::deque<int> moves_time_micro{};
        std::deque<int> moves_per_iteration{};

        std::deque< std::pair<int, int> > moves_failtime_array{}; //

        bool failed = false;
        bool first_move = true;
        bool human_incorrect_move = false;

        double scale_factor{};
        // double max_waiting_time{};
        int time_for_move = 16000; //16k microseconds == 16ms, about 1/30th of a s.
        int iteration_id{};

        std::chrono::system_clock::time_point time_obstacle_spotted; //moment in time when next move was generated
        std::chrono::system_clock::time_point time_move_received; //moment in time when key press was received by q
        game_message message_out; 
        game_message message_in;
        char current_key{};

    public:
        GameProcess();
        ~GameProcess() {}

        void waitForSignal();
        void validateSignal();
        void generateNextMove();
        void sendPing();

        void resetGame();
        std::string prepareOutputFile();
        void saveRunsToFile(std::string file_name); 

        void saveOnExit();

        [[noreturn]] void run();

        // static void* gameRunner(void* context);
        // [[noreturn]] void bitchImRunning();
};

#endif