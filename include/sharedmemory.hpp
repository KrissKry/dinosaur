#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP

#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "util.hpp"
#include <iostream>
struct data {
    int id{};
    unsigned char buf[WIDTH * HEIGHT * 3 + 1]{};

};
// int frame_memory_id{};

class SharedMemory {

    private:
        //wskaznik na pamiec chuj wie co jeszcze
        int id_cnt{};
        int frame_memory_id{};
        void* mem_ptr = nullptr;
        sem_t* producer;
        sem_t* consumer;
    public:

        SharedMemory(bool will_write);
        ~SharedMemory();

        void push(unsigned char *buf);
        bool hasItems();
        void pop(unsigned char *buf);
};


#endif


