#ifndef SHAREDQUEUE_HPP
#define SHAREDQUEUE_HPP

#include <mqueue.h>
#include <chrono>
#include "util.hpp"


//https://www.softprayog.in/programming/interprocess-communication-using-posix-message-queues-in-linux?PageSpeed=noscript

typedef struct coords_message {
    int x{}, y{};
    std::chrono::system_clock::time_point timestamp{};
};

// typedef struct game_message {
    // int y{}; //required y
    // std::chrono::system_clock::time_point deadline{};
// }

class SharedQueue {

    private:

        mqd_t mq;
        size_t msg_size;

    public:

        SharedQueue(bool will_write);
        ~SharedQueue();

        template <typename T>
        void push(T* msg);

        template <typename T>
        void pop(T* msg);



        // void push(message msg); //ablo strukture msg (chyba lepiej tak będzie)
        // message pop();
};

#endif;