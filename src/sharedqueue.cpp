
#include "sharedqueue.hpp"




SharedQueue::SharedQueue(bool will_write) {
    // mq = mq_open(QUEUE_NAME, )
    if (will_write)
        mq = mq_open(QUEUE_NAME, O_WRONLY | O_NONBLOCK);
    else
        mq = mq_open(QUEUE_NAME, O_RDONLY | O_NONBLOCK);


    mq_attr msg_attributes{};

    mq_getattr(mq, &msg_attributes);
    msg_size = msg_attributes.mq_msgsize;
}


template <typename T>
void SharedQueue::push(T* msg) {

    mq_send(mq, (const char*)msg, msg_size, 0);
}


template <typename T>
void SharedQueue::pop(T* msg) {

    char buffer[msg_size];
    int result = mq_receive(mq, &buf[0], msg_size, 0);

    if (result != -1) {
        std::cout << "ay";
        memcpy(&msg, buffer, sizeof(T));
    } else {
        &msg = nullptr;
    }
}

