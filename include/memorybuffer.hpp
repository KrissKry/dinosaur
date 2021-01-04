#ifndef SCZR_MEMORYBUFFER_H
#define SCZR_MEMORYBUFFER_H

// #include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "util.hpp"
using namespace boost::interprocess;

//https://www.boost.org/doc/libs/1_55_0/doc/html/interprocess/quick_guide.html tutaj to szukanie bufora typu getBuffer()

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
            std::cout << "[I]Granted.\n";
            // producer = named_semaphore(open_only_t(), PROD_SEM);
            // consumer = named_semaphore(open_only_t(), CONS_SEM);
        }
        ~MemoryBuffer() {
            named_semaphore::remove(PROD_SEM);
            named_semaphore::remove(CONS_SEM);
        }

        void push(int size, char* buf) {

            producer.wait();
            std::cout << "[P] pushing " << size << " bytes to memory\n";
            std::memcpy(region.get_address(), buf, size);
            std::cout << "[P] pushed.\n";
            consumer.post();
        }

        void pop(int size, char* buf) {

            consumer.wait();
            std::cout << "[F] reading " << size << " bytes from memory\n";
            memcpy(buf, region.get_address(), size);
            std::cout << "[F] read.\n";
            producer.post();
        }

    private:

        mapped_region region;
        shared_memory_object shm;
        named_semaphore producer = named_semaphore(open_or_create_t(), PROD_SEM, 1);
        named_semaphore consumer = named_semaphore(open_or_create_t(), CONS_SEM, 0);

};

// class MemoryBuffer {
    
//     private:

//         managed_shared_memory mem_segment = managed_shared_memory(open_or_create, FRAME_SHM, WIDTH * HEIGHT * 3 * FRAME_SHM_CNT);
//         auto region = mapped_region{mem_segment, read_write};
//         static inline bool available = true;
    
//     public:

//         static void destroy() {
//             available = false;
//             mem_segment.destroy<MemoryBuffer>(unique_instance);
//         }

//         static MemoryBuffer* getBuffer() {
//             std::pair<Buffer *, std::size_t> shared_memory = mem_segment.find<Buffer>(unique_instance);

//             //no buffer created yet
//             if ( shared_memory.first == nullptr) {
//                 return mem_segment.construct<Buffer>(unique_instance)();
//             //buffer found in memory segment
//             } else {
//                 return shared_memory.first;
//             }
//         }
//         void push(int id, char *buf) {
//             named_semaphore producer(open_only_t(), PROD_SEM);
//             named_semaphore consumer(open_only_t(), CONS_SEM);
//             producer.wait();

//             mem_segment.construct<char>(id)(buf, mem_segment.get_sha)

//         }
// };

#endif