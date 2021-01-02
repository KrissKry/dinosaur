#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP

#include "util.hpp"
struct data {
    char* buf[WIDTH * HEIGHT * 3]{};

};

class SharedMemory {

    private:
        //wskaznik na pamiec chuj wie co jeszcze
        // sem_t q_access;
    public:

        SharedMemory();
        ~SharedMemory();

        void push(char *buf);
        bool hasItems();
        char* pop();
};


#endif;


