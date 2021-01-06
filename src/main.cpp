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



    
    // MessageQueue* mq1 = new MessageQueue(true, MQ_FILTER_CLIENT);
    // MessageQueue* mq2 = new MessageQueue(false, MQ_FILTER_CLIENT);

    // MessageQueue* mq3 = new MessageQueue(true, MQ_PRODUCER_GAME);
    // MessageQueue* mq4 = new MessageQueue(false, MQ_PRODUCER_GAME);


    // coords_message cords_out;
    // coords_message cords_in;

    // game_message key_out;
    // game_message key_in;

    // int cnt = 0;

    // while (cnt < 10) {
    //     cords_out.x = cnt;
    //     cords_out.y = cords_out.x * 2;
    //     std::cout <<"SEND OUT: " << cords_out.x << " " << cords_out.y << std::endl;
    //     mq1->push(&cords_out);
    //     cords_out.x += 1;
    //     std::cout <<"SEND OUT: " << cords_out.x << " " << cords_out.y << std::endl;
    //     mq1->push(&cords_out);
    //     mq2->pop(&cords_in);
    //     std::cout <<"RECEIVE: " << cords_in.x << " " << cords_in.y << std::endl;

    //     cnt++;
    // }
    boost::interprocess::message_queue::remove(MQ_CLIENT_GAME);
    boost::interprocess::message_queue::remove(MQ_FILTER_CLIENT);
    boost::interprocess::message_queue::remove(MQ_PRODUCER_GAME);
    boost::interprocess::shared_memory_object::remove(FRAME_SHM);
    // shm = shared_memory_object ( create_only, FRAME_SHM, read_write); 
    boost::interprocess::shared_memory_object( create_only, FRAME_SHM, read_write);
    boost::interprocess::message_queue(create_only, MQ_PRODUCER_GAME, 10, sizeof(game_message) );
    boost::interprocess::message_queue(create_only, MQ_CLIENT_GAME, 10, sizeof(game_message) );
    boost::interprocess::message_queue(create_only, MQ_FILTER_CLIENT, 10, sizeof(coords_message) );
    pid_t producer_id;
    pid_t filter_id;
    pid_t controller_id;
    pid_t game_id;


    //chyba w tej kolejnosci musza byc x dddd
    game_id = runChild<GameProcess>();
    std::cout << "[I] Game_id: " << game_id << std::endl;

    controller_id = runChild<ControlProcess>();
    std::cout << "[I] Controller_id: " << controller_id << std::endl;
    
    producer_id = runChild<ProducerProcess>();
    std::cout << "[I] Prod_id: " << producer_id << std::endl;

    filter_id = runChild<FilterProcess>();
    std::cout << "[I] Filter_id: " << filter_id << std::endl;



    int choice;

    std::cin >> choice;

    switch(choice) {
        case 0: {
            killChild(controller_id);
            killChild(producer_id);
            killChild(filter_id);
            killChild(game_id);
            std::cout << "Executed order 66" << std::endl << std::flush;
            break;
        }
        default: {
            std::cout << "bruh";
            break;
        }

    }
    // pid_t producer_id, filter_id, controller_id, game_id;
    // spawnChildren(producer_id, filter_id, controller_id);

    // setupChildren(SCHED_OPTIONS::FIFO, producer_id, filter_id, controller_id); 
    // printChildren(producer_id, filter_id, controller_id);
    return 0;
}

// void chooseAction() {
//     int choice;

//     cin >>choice;

//     switch(choice): {
//         case 0: {
//             killChild(controller)
//         }

//     }
// }
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
        // std::cout << "[I] Attempting to run child\n";
        child.run();
        // std::cout << "child running\n";

    } 
    
    return result;

}