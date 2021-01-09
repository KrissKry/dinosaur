#include <iostream>
#include <signal.h>

#include "../include/controlprocess.hpp"
#include "../include/filterprocess.hpp"
#include "../include/producerprocess.hpp"
#include "../include/gameprocess.hpp"


enum SCHED_OPTIONS { FIFO, DEADLINE, DEFAULT };

void spawnChildren(pid_t &producer_id, pid_t &filter_id, pid_t &controller_id, pid_t& game_id);
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id, pid_t game_id);
void printChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id, pid_t& game_id);
void killChild(pid_t& process_id);
template <typename T>
pid_t runChild();
void resetBoost();





int main(int argc, char* argv[])
{

    pid_t producer_id, filter_id, controller_id, game_id;
    
    resetBoost();
    spawnChildren(producer_id, filter_id, controller_id, game_id);
    setupChildren(SCHED_OPTIONS::DEFAULT, producer_id, filter_id, controller_id, game_id); 
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
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id, pid_t game_id) {

            //to-do set process scheduler adequately
    int result{};
    sched_param params{1};
    switch(sched){
        case SCHED_OPTIONS::FIFO: {

            result |= sched_setscheduler(producer_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(filter_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(controller_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(game_id, SCHED_FIFO, &params);

        }

        case SCHED_OPTIONS::DEADLINE: {

            result |= sched_setscheduler(producer_id, SCHED_DEADLINE, &params);
            result |= sched_setscheduler(filter_id, SCHED_DEADLINE, &params);
            result |= sched_setscheduler(controller_id, SCHED_DEADLINE, &params);
            result |= sched_setscheduler(controller_id, SCHED_DEADLINE, &params);

        }

        case SCHED_OPTIONS::DEFAULT: {
            //yee
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

    
}



void killChild(pid_t& process_id) {

    //what's worse than a rapist
    //boom
    //a child
    kill(process_id, SIGTERM);
}



//returns a pid_t type for given object that will be running as a separate process
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