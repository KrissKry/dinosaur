#include <iostream>
#include <sched.h>
#include <unistd.h>
#include "../include/controlprocess.hpp"
#include "../include/filterprocess.hpp"
#include "../include/producerprocess.hpp"

enum SCHED_OPTIONS { FIFO, DEADLINE, DEFAULT };

void spawnChildren(pid_t &producer_id, pid_t &filter_id, pid_t &controller_id);
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id);
void printChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id);
void killChild(pid_t& process_id);
template <typename T>
pid_t runChild();


int main(int argc, char* argv[])
{

    pid_t producer_id, filter_id, controller_id;
    spawnChildren(producer_id, filter_id, controller_id);

    setupChildren(SCHED_OPTIONS::FIFO, producer_id, filter_id, controller_id); 
    printChildren(producer_id, filter_id, controller_id);
    return 0;
}

//print pid_t of each child
void printChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id) {

    std::cout << "Prod_id: " << producer_id << std::endl;
    std::cout << "Filter_id: " << filter_id << std::endl;
    std::cout << "Controller_id: " << controller_id  << std::endl;
}

//fork children processes
void spawnChildren(pid_t &producer_id, pid_t &filter_id, pid_t &controller_id) {

    controller_id = runChild<ControlProcess>();
    filter_id = runChild<FilterProcess>();
    producer_id = runChild<ProducerProcess>();

    if ( controller_id == 0 || filter_id == 0 || producer_id == 0) {
        std::cerr<<"Failed at forking children" << std::endl;
        //to-do invoke function killing all program's processes
        killChild(controller_id);
        killChild(filter_id);
        killChild(producer_id);
    }

}

//set correct scheduler for our processes
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id) {

            //to-do set process scheduler adequately
    int result{};

    switch(sched){
        case SCHED_OPTIONS::FIFO: {

            result |= sched_setscheduler(producer_id, SCHED_FIFO, XD);
            result |= sched_setscheduler(filter_id, SCHED_FIFO, XD);
            result |= sched_setscheduler(controller_id, SCHED_FIFO, XD);

        }

        case SCHED_OPTIONS::DEADLINE: {

            result |= sched_setscheduler(producer_id, SCHED_DEADLINE, XD);
            result |= sched_setscheduler(filter_id, SCHED_DEADLINE, XD);
            result |= sched_setscheduler(controller_id, SCHED_DEADLINE, XD);

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