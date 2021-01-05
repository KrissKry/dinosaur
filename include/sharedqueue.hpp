#ifndef SHAREDQUEUE_HPP
#define SHAREDQUEUE_HPP

#include <mqueue.h>
#include <chrono>
#include "util.hpp"


//https://www.softprayog.in/programming/interprocess-communication-using-posix-message-queues-in-linux?PageSpeed=noscript

struct coords_message {
    int x{}, y{}; //coords of the object found
    std::chrono::system_clock::time_point timestamp{};
};

struct game_message {
    
    int y{}; //required y
    //subject to future changes .-.
    std::chrono::system_clock::time_point deadline{};
};

class SharedQueue {

    private:

        mqd_t mq;
        size_t msg_size;

    public:

        SharedQueue(bool will_write, const char* queue_name) {
            if (will_write)
                // mq = mq_open(QUEUE_NAME, O_WRONLY | O_NONBLOCK);
                mq = mq_open(queue_name, O_WRONLY);
            else
                // mq = mq_open(QUEUE_NAME, O_RDONLY | O_NONBLOCK);
                mq = mq_open(queue_name, O_RDONLY);


            mq_attr msg_attributes{};

            mq_getattr(mq, &msg_attributes);
            msg_size = msg_attributes.mq_msgsize;
        }

        ~SharedQueue() {
            mq_close(mq);
        }

        template <typename T>
        void push(T* msg_struct) {

            mq_send(mq, (const char *) msg_struct, sizeof(T), 0);
        }

        template <typename T>
        void pop(T* msg_struct) {

            int result = mq_receive(mq, (char *) msg_struct, sizeof(T), NULL);
        }

};

#endif