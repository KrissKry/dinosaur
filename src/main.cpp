#include <iostream>
#include <signal.h>
#include <thread>

#include "../include/controlprocess.hpp"
#include "../include/filterprocess.hpp"
#include "../include/producerprocess.hpp"
#include "../include/gameprocess.hpp"


enum SCHED_OPTIONS { FIFO, RR, DEFAULT };

void spawnChildren(pid_t &producer_id, pid_t &filter_id, pid_t &controller_id, pid_t& game_id);
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id, pid_t game_id, bool core_bound);
void printChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id, pid_t& game_id);
void killChild(pid_t& process_id);
template <typename T>
pid_t runChild();
void resetBoost();

void schedMinMaxValues() {
    int fifo = sched_get_priority_max( SCHED_FIFO );
    int fifo_min = sched_get_priority_min( SCHED_FIFO );
    int rr = sched_get_priority_max( SCHED_RR );
    int rr_min = sched_get_priority_min( SCHED_RR );

    int def = sched_get_priority_max( SCHED_OTHER );
    int def_min = sched_get_priority_min( SCHED_OTHER );

    std::cout << "MAX: OTHER: " << def << " | FIFO: " << fifo << " | RR: " << rr << std::endl << std::flush;

    std::cout << "MIN: OTHER: " << def_min << " | FIFO: " << fifo_min << " | RR: " << rr_min << std::endl << std::flush;
}



int main(int argc, char* argv[])
{

    schedMinMaxValues();
    pid_t producer_id, filter_id, controller_id, game_id;
    
    // checkMax();
    resetBoost();
    spawnChildren(producer_id, filter_id, controller_id, game_id);
    setupChildren(SCHED_OPTIONS::RR, producer_id, filter_id, controller_id, game_id, CORE_BOUND); 
    printChildren(producer_id, filter_id, controller_id, game_id);

    
    int choice;
    std::cin >> choice;

    switch(choice) {
        default: {
            killChild(controller_id);
            killChild(producer_id);
            killChild(filter_id);
            killChild(game_id);
            std::cout << "Executed order 66" << std::endl << std::flush;
            break;
        }
    }
    

    return 0;
}






void printChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id, pid_t& game_id) {

    std::cout << "Prod_id: " << producer_id << std::endl << std::flush;
    std::cout << "Filter_id: " << filter_id << std::endl << std::flush;
    std::cout << "Controller_id: " << controller_id  << std::endl << std::flush;
    std::cout << "Game_id: " << game_id << std::endl << std::flush;
}



void spawnChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id, pid_t& game_id) {

    game_id = runChild<GameProcess>();
    controller_id = runChild<ControlProcess>();
    producer_id = runChild<ProducerProcess>();
    filter_id = runChild<FilterProcess>();

    if ( controller_id == 0 || filter_id == 0 || producer_id == 0 || game_id == 0) {
        std::cerr<<"[!!!] Failed at forking children. Aborting..." << std::endl << std::flush;

        killChild(controller_id);
        killChild(filter_id);
        killChild(producer_id);
        killChild(game_id);
    }

}



//set correct scheduler for our processes
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id, pid_t game_id, bool core_bound) {

    int result{};
    sched_param params{99};
    sched_param def_param{};
    switch(sched){
        case SCHED_OPTIONS::FIFO: {
            std::cout << "[MAIN] Setting FIFO SCHED" << std::endl << std::flush;
            result |= sched_setscheduler(producer_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(filter_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(controller_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(game_id, SCHED_FIFO, &params);
            break;
        }

        case SCHED_OPTIONS::RR: {
            std::cout << "[MAIN] Setting RR SCHED" << std::endl << std::flush;
            result |= sched_setscheduler(producer_id, SCHED_RR, &params);
            result |= sched_setscheduler(filter_id, SCHED_RR, &params);
            result |= sched_setscheduler(controller_id, SCHED_RR, &params);
            result |= sched_setscheduler(game_id, SCHED_RR, &params);
            break;
        }

        case SCHED_OPTIONS::DEFAULT: {

            std::cout << "[MAIN] Setting OTHER SCHED" << std::endl << std::flush;
            result |= sched_setscheduler(producer_id, SCHED_OTHER, &def_param);
            result |= sched_setscheduler(filter_id, SCHED_OTHER, &def_param);
            result |= sched_setscheduler(controller_id, SCHED_OTHER, &def_param);
            result |= sched_setscheduler(game_id, SCHED_OTHER, &def_param);
            break;
        }

        default: {
            
            break;
        }
    }

    if (result != 0) {
        std::cout<< strerror(errno) << std::endl;
        std::cout << "bruh";
        
    }

    if (core_bound) {
        result = 0;

        cpu_set_t cpu;



        CPU_ZERO(&cpu);
        CPU_SET(1, &cpu);
        result |= sched_setaffinity(producer_id, sizeof(cpu_set_t), &cpu);


        // CPU_ZERO(&cpu);
        // CPU_SET(1, &cpu);
        result |= sched_setaffinity(filter_id, sizeof(cpu_set_t), &cpu);



        // CPU_ZERO(&cpu);
        // CPU_SET(2, &cpu);
        result |= sched_setaffinity(controller_id, sizeof(cpu_set_t), &cpu);


        // CPU_ZERO(&cpu);
        // CPU_SET(3, &cpu);
        result |= sched_setaffinity(game_id, sizeof(cpu_set_t), &cpu);

    }

    if( result != 0 ) {
        std::cout << strerror(errno) << std::endl;
        std::cout << "bruh2";
    }

    
}



void killChild(pid_t& process_id) {

    //what's worse than a rapist
    //boom
    //a child
    kill(process_id, SIGTERM);
}



//returns a pid_t type for given object that will be running as a separate process
// runChild<GameProcess>();
template <typename T>
pid_t runChild() {

    pid_t result = fork();

    if (result == 0) {
        T child;

        child.run();


    } 
    
    return result;

}



void resetBoost() {
    //remove possible leftovers from previous run
    boost::interprocess::message_queue::remove(MQ_CLIENT_GAME);
    boost::interprocess::message_queue::remove(MQ_FILTER_CLIENT);
    boost::interprocess::message_queue::remove(MQ_PRODUCER_GAME);
    boost::interprocess::shared_memory_object::remove(FRAME_SHM);
    boost::interprocess::named_semaphore::remove(PROD_SEM);
    boost::interprocess::named_semaphore::remove(CONS_SEM);


    //init shared memory and message queues
    boost::interprocess::shared_memory_object( create_only, FRAME_SHM, read_write);
    boost::interprocess::message_queue(create_only, MQ_PRODUCER_GAME, 10, sizeof(game_message) );
    boost::interprocess::message_queue(create_only, MQ_CLIENT_GAME, 10, sizeof(game_message) );
    boost::interprocess::message_queue(create_only, MQ_FILTER_CLIENT, 10, sizeof(coords_message) );
}