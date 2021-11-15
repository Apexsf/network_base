#ifndef THREAD_H
#define THREAD_H

#include <functional>
#include <pthread.h>
#include "utils.hpp"



class thread{
public:
    using TASK = std::function<void()>;
    
    //non copyable
    thread(const thread&) = delete;
    thread& operator= (const thread&) = delete;

    thread(TASK task);
    ~thread();

    void start();
    void join();
    pthread_t get_pthread_t();
    bool is_running();

    // void detach();

    static void* run (void* args);



private:
    pthread_t m_pthread_t;
    TASK m_task;
    bool m_running;
};


#endif