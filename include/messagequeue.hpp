#ifndef SCZR_MESSAGEQUEUE
#define SCZR_MESSAGEQUEUE

#include <boost/interprocess/ipc/message_queue.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include "util.hpp"

namespace bip = boost::interprocess;

struct coords_message {
    int x{}, y{}; //coords of the object found
    std::chrono::system_clock::time_point timestamp{};
};

struct game_message {
    
    // int y{}; //required y
    //subject to future changes .-.

    char key{}; //required / or pressed key
    std::chrono::system_clock::time_point deadline{};
};



class MessageQueue {

    private:

        bool is_allowed_write;
        bip::message_queue* mq;
        std::string name;

    public:
        
        MessageQueue(bool will_write, const char* queue_name) {

            name = queue_name;
            is_allowed_write = will_write;

            if (will_write) {

                
                std::cout << "[QUEUE] Resetting message queue " << queue_name << " and acquiring WRITE/READ access" << std::endl << std::flush;
                // bip::message_queue::remove(queue_name);
                // mq->remove(queue_name);
                const int max_msgs = 10;
                int msg_size{};

                if (queue_name == MQ_FILTER_CLIENT)
                    msg_size = sizeof(coords_message);
                else    
                    msg_size = sizeof(game_message);
                    
                mq = new bip::message_queue(bip::open_or_create, queue_name, max_msgs, msg_size);
                
                // mq->get_max_msg();
                

            } else {

                
                usleep(10000000);
                std::cout << "[QUEUE] Opening message queue " << queue_name << " with READ access" << std::endl << std::flush;     

                mq = new bip::message_queue(bip::open_only, queue_name);


            }

            if (mq->get_max_msg() != 0)
                std::cout << "[QUEUE] Success" << std::endl << std::flush;
            // is_allowed_write = will_write;
            // const int max_msgs = 10;
            // const int msg_size = sizeof(coords_message);

            // mq = message_queue(open_or_create, queue_name, 10, 16);
            // bip::message_queue::remove(queue_name);

            // mq = new bip::message_queue(bip::open_or_create, queue_name, max_msgs, msg_size);
            // std::cout << "Max msgs: " << mq->get_max_msg() << " with max size: " << mq->get_max_msg_size() << std::endl << std::flush;
            // std::cout << "Current msgs in " << name << " " << mq->get_num_msg() << std::endl<<std::flush;
            // std::cout << "[I] Success" << std::endl;
            // usleep(500000);

        }
        ~MessageQueue() {
            delete mq;
        }


        template <typename T>
        void push(T* msg_struct) {
            
            if (is_allowed_write)
                mq->send( msg_struct, sizeof(T), 0 );
            else
                std::cout << "[!!!!!] Attempted to write to " << name << " without privileges" << std::endl;

            if (SHMQ_LOG)
                std::cout << "[QUEUE] PUSHED: Current msgs in " << name << " " << mq->get_num_msg() << std::endl<<std::flush;
        }


        template <typename T>
        void pop(T* msg_struct) {
            if (SHMQ_LOG)
                std::cout << "[QUEUE] PRE-POP: Current msgs in "<< name << " " << mq->get_num_msg() << std::endl<<std::flush;

            bip::message_queue::size_type bytes = 0;
            unsigned int priority = 0;
            // mq.receive( msg_struct, sizeof(msg_struct), sizeof(T), NULL);
            mq->receive(msg_struct, sizeof(T), bytes, priority);
            if (SHMQ_LOG)
                std::cout << "[QUEUE] POPPED: Current msgs in "<< name << " " << mq->get_num_msg() << std::endl<<std::flush;
        }
};

#endif