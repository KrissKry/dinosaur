#ifndef SCZR_MEMORYBUFFER_H
#define SCZR_MEMORYBUFFER_H

// #include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "util.hpp"
using namespace boost::interprocess;


struct frame_data {
    int id{};
    char* pixels = new char[WIDTH*HEIGHT*3];
};

class MemoryBuffer {

    public:

        MemoryBuffer(bool will_write) {
            if (will_write) {

                std::cout<< "[I] Attempting to give mem access to producer...\n";
                shm = shared_memory_object (open_or_create, FRAME_SHM, read_write); 
                shm.truncate(WIDTH * HEIGHT * 3);           

                region = mapped_region(shm, read_write);

            } else {
                std::cout << "[I] Attempting to give mem access to filter...\n";

                shm = shared_memory_object( open_only, FRAME_SHM, read_only );
                region = mapped_region(shm, read_only);

            }
            std::cout << "[I] Granted.\n";

        }

        ~MemoryBuffer() {
            named_semaphore::remove(PROD_SEM);
            named_semaphore::remove(CONS_SEM);
        }

        void push(int size, char* buf) {

            producer.wait();
            std::cout << "[I] pushing " << size << " bytes to memory\n";
            std::memcpy(region.get_address(), buf, size);
            // std::cout << "[P] pushed.\n";
            consumer.post();
        }

        void pop(int size, char* buf) {

            consumer.wait();
            std::cout << "[I] reading " << size << " bytes from memory\n";
            memcpy(buf, region.get_address(), size);
            // std::cout << "[F] read.\n";
            producer.post();
        }

    private:

        mapped_region region;
        shared_memory_object shm;
        named_semaphore producer = named_semaphore(open_or_create_t(), PROD_SEM, 1);
        named_semaphore consumer = named_semaphore(open_or_create_t(), CONS_SEM, 0);

};


#endif