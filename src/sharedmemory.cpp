#include "../include/sharedmemory.hpp"



SharedMemory::SharedMemory(bool will_write) {


    producer = sem_open(PROD_SEM, O_CREAT, 0644, 1);
    consumer = sem_open(CONS_SEM, O_CREAT, 0644, 0);
    std::cout << "copa cabana\n";
    
    if (will_write) {
        //producer here, has to create mem first
        // sem_wait(producer);

        frame_memory_id = shm_open(FRAME_SHM, O_CREAT | O_WRONLY, 0666);

        int result = ftruncate(frame_memory_id, sizeof(data) );

        if (result < 0)
            std::cout << "result fked?\n";
        
        mem_ptr = mmap(0, sizeof(data), PROT_WRITE, MAP_SHARED, frame_memory_id, 0);
    
        sem_post(consumer);

    } else {
        //wont open shared memory without producer creating it first
        sem_wait(consumer);

        frame_memory_id = shm_open(FRAME_SHM, O_RDONLY, 0666);

        mem_ptr = mmap(0, sizeof(data), PROT_READ, MAP_SHARED, frame_memory_id, 0);

        sem_post(producer);
    }

    std::cout << "end of shm init\n" << std::fflush;
}

SharedMemory::~SharedMemory() {
    sem_unlink(PROD_SEM);
    sem_unlink(CONS_SEM);
}
//narazie na jedna klatke w pamięci projekt
void SharedMemory::push(unsigned char* buf) {

    sem_wait(producer);
    //write id to memory
    sprintf( static_cast<char*>(mem_ptr), "%d", id_cnt);
    
    //nie dziala bo arytmetyka na wskaźniku void nie śmiga mordo
    // mem_ptr += sizeof(id_cnt);
    mem_ptr = static_cast<char*>(mem_ptr) + sizeof(id_cnt);

    //write frame data to memory
    sprintf( static_cast<char*>(mem_ptr), "%s", buf );
    

    mem_ptr = static_cast<char*>(mem_ptr) - sizeof(id_cnt);

    sem_post(consumer);
}


void SharedMemory::pop(unsigned char *buf) {

    sem_wait(consumer);
    int* id;
    memcpy(id, static_cast<char*>(mem_ptr), sizeof(id));


    printf("Frame %d is being read\n", *id);

    memcpy(buf, static_cast<unsigned char*>(mem_ptr) + sizeof(int), sizeof(data) - sizeof(int));

    printf("%s\n", buf);

    sem_post(producer);
}