#ifndef SCZR_MEMORYBUFFER_H
#define SCZR_MEMORYBUFFER_H

// #include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "util.hpp"
#include <fcntl.h>
using namespace boost::interprocess;


// struct frame_data {
//     int id{};
//     char* pixels = new char[FRAME_SIZE];
// };

class MemoryBuffer {

    public:

        MemoryBuffer(bool will_write) {
            if (will_write) {

                std::cout<< "[MEM] Resetting and acquiring shared memory READ/WRITE access..." << std::endl << std::flush;

                // shared_memory_object::remove(FRAME_SHM);

                shm = shared_memory_object ( open_or_create, FRAME_SHM, read_write); 
                shm.truncate(FRAME_SIZE);           

                region = mapped_region(shm, read_write);

            } else {
                usleep(5000000);

                std::cout << "[MEM] Acquiring shared memory READ access..." << std::endl << std::flush;

                shm = shared_memory_object( open_only, FRAME_SHM, read_only );
                region = mapped_region(shm, read_only);

            }
            std::cout << "[MEM] Success" << std::endl << std::flush;

        }

        ~MemoryBuffer() {
            named_semaphore::remove(PROD_SEM);
            named_semaphore::remove(CONS_SEM);
        }

        void push(int size, char* buf) {

            producer.wait();

            if (SHMQ_LOG)
                std::cout << "[MEM] pushing " << size << " bytes to memory\n";

            std::memcpy(region.get_address(), buf, size);
            consumer.post();
        }

        void pop(int size, char* buf) {

            consumer.wait();
            
            if (SHMQ_LOG)
                std::cout << "[MEM] reading " << size << " bytes from memory\n";

            memcpy(buf, region.get_address(), size);
            producer.post();
        }

    private:

        mapped_region region;
        shared_memory_object shm;
        named_semaphore producer = named_semaphore(open_or_create_t(), PROD_SEM, 1);
        named_semaphore consumer = named_semaphore(open_or_create_t(), CONS_SEM, 0);

};


#endif