#include <iostream>

#include <signal.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "../include/controlprocess.hpp"
#include "../include/filterprocess.hpp"
#include "../include/producerprocess.hpp"
// #include "../include/messagequeue.hpp"
#include "../include/gameprocess.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <random>

enum SCHED_OPTIONS { FIFO, DEADLINE, DEFAULT };

void spawnChildren(pid_t &producer_id, pid_t &filter_id, pid_t &controller_id);
void setupChildren(SCHED_OPTIONS sched, pid_t producer_id, pid_t filter_id, pid_t controller_id);
void printChildren(pid_t& producer_id, pid_t& filter_id, pid_t& controller_id);
void killChild(pid_t& process_id);
template <typename T>
pid_t runChild();


int main(int argc, char* argv[])
{
    //TEST W JEDNYM WATKU JEDEN PO DRUGIM SYNCHRO NIEPOTRZEBNA
    // ProducerProcess* Producer = new ProducerProcess();
    // FilterProcess* Filter = new FilterProcess();
    // Producer->TestOnce();
    // Filter->TestOnce();
    // delete Producer;
    // delete Filter;

    //JA WTEDY ROBIE NUMERY
    // struct mq_attr attr;
    // attr.mq_flags =0;
    // attr.mq_maxmsg=10;
    // attr.mq_msgsize = sizeof(coords_message);
    // attr.mq_curmsgs = 0;
    // // mq_unlink(MQ_FILTER_CLIENT);
    // mqd_t meq = mq_open("xddd", O_CREAT | O_RDWR, 0666, NULL);
    // mq_setattr(meq, &attr, NULL);
     

    // mq_attr at2;
    // mq_getattr(meq, &at2);
    // std::cout << at2.mq_maxmsg << " " << at2.mq_msgsize << std::endl;



    // SharedQueue* sh1 = new SharedQueue(true, MQ_FILTER_CLIENT);
    // SharedQueue* sh2 = new SharedQueue(false, MQ_FILTER_CLIENT);
    // MessageQueue* mq1 = new MessageQueue(true, MQ_FILTER_CLIENT);
    // MessageQueue* mq2 = new MessageQueue(false, MQ_FILTER_CLIENT);
    // MessageQueue mq1(true, MQ_FILTER_CLIENT);
    // MessageQueue mq2(false, MQ_FILTER_CLIENT);
    // coords_message coord1;
    // coords_message coord2;
    // std::cout << "bruh\n";
    // coord1.x = 5;
    // coord1.y = 2;
    // coord2.y = 1;
    // std::cout << "Sending: " << coord2.x << " " << coord2.y << std::endl;
    // mq1->push(&coord2);
    // mq2->pop(&coord1);
    // std::cout << "Got: " << coord1.x << " " << coord1.y << std::endl;
//     enum class Key_Press {
//     W = (int)'w',
//     S = (int)'s',
//     NON = (int)' '


        // std::cout <<
        // return (char)(Key_Press( dist(m_mt) ));
    pid_t producer_id;
    pid_t filter_id;
    pid_t controller_id;
    pid_t game_id;
    game_id = runChild<GameProcess>();
    std::cout << "[I] Game_id: " << game_id << std::endl;
    producer_id = runChild<ProducerProcess>();
    std::cout << "[I] Prod_id: " << producer_id << std::endl;
    filter_id = runChild<FilterProcess>();
    std::cout << "[I] Filter_id: " << filter_id << std::endl;
    controller_id = runChild<ControlProcess>();
    std::cout << "[I] Controller_id: " << controller_id << std::endl;


    // pid_t producer_id, filter_id, controller_id;
    // spawnChildren(producer_id, filter_id, controller_id);

    // setupChildren(SCHED_OPTIONS::FIFO, producer_id, filter_id, controller_id); 
    // printChildren(producer_id, filter_id, controller_id);
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

    // controller_id = runChild<ControlProcess>();
    filter_id = runChild<FilterProcess>();
    producer_id = runChild<ProducerProcess>();

    if ( controller_id == 0 || filter_id == 0 || producer_id == 0) {
        std::cerr<<"[!!!] Failed at forking children" << std::endl;
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
    sched_param params{1};
    switch(sched){
        case SCHED_OPTIONS::FIFO: {

            result |= sched_setscheduler(producer_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(filter_id, SCHED_FIFO, &params);
            result |= sched_setscheduler(controller_id, SCHED_FIFO, &params);

        }

        case SCHED_OPTIONS::DEADLINE: {

            result |= sched_setscheduler(producer_id, SCHED_DEADLINE, &params);
            result |= sched_setscheduler(filter_id, SCHED_DEADLINE, &params);
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
        std::cout << "[I] Attempting to run child\n";
        child.run();
        // std::cout << "child running\n";

    } 
    
    return result;

}