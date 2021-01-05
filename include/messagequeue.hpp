#ifndef SCZR_MESSAGEQUEUE
#define SCZR_MESSAGEQUEUE

#include <boost/interprocess/ipc/message_queue.hpp>
#include <chrono>

namespace bip = boost::interprocess;

struct coords_message {
    int x{}, y{}; //coords of the object found
    std::chrono::system_clock::time_point timestamp{};
};

struct game_message {
    
    int y{}; //required y
    //subject to future changes .-.
    std::chrono::system_clock::time_point deadline{};
};



class MessageQueue {

    private:

        bool is_allowed_write;
        bip::message_queue* mq;

    public:
        
        MessageQueue(bool will_write, const char* queue_name) {


            if (will_write)
                std::cout << "[I] Opening message queue " << queue_name << " with WRITE/READ" << std::endl;
            else
                std::cout << "[I] Opening message queue " << queue_name << " with READ" << std::endl;


            is_allowed_write = will_write;
            const int max_msgs = 10;
            const int msg_size = sizeof(coords_message);

            // mq = message_queue(open_or_create, queue_name, 10, 16);
            // bip::message_queue::remove(queue_name);

            mq = new bip::message_queue(bip::open_or_create, queue_name, max_msgs, msg_size);

            std::cout << "[I] Success" << std::endl;

        }
        ~MessageQueue() {
            delete mq;
        }


        template <typename T>
        void push(T* msg_struct) {

            if (is_allowed_write)
                mq->send( msg_struct, sizeof(T), 0 );
            else
                std::cout << "[!!!] Attempted to write to Message Q without privileges" << std::endl;
        }


        template <typename T>
        void pop(T* msg_struct) {

            bip::message_queue::size_type bytes = 0;
            unsigned int priority = 0;
            // mq.receive( msg_struct, sizeof(msg_struct), sizeof(T), NULL);
            mq->receive(msg_struct, sizeof(T), bytes, priority);
        }
};

#endif