#ifndef CONDITION_H
#define CONDITION_H

#include <functional>
#include <time.h>
#include <pthread.h>
#include "mutex.hpp"
#include "utils.hpp"

class condition {
public:
    explicit condition(mutex& m): m_mutex(m){
        return_value_check_equal(pthread_cond_init(&m_cond_t, NULL) , 0 );
    }

    ~condition(){
        return_value_check_equal(pthread_cond_destroy(&m_cond_t) , 0 );
    }

    mutex& get_mutex(){
        return m_mutex;
    }

    void wait(){
        return_value_check_equal(pthread_cond_wait(&m_cond_t, m_mutex.get_mutex_t()) , 0 );
    }

    void wait(std::function<bool()> func) {
        while (func()) {
            wait();
        }
    }

    void time_wait(const struct timespec* tm) {
        int ret = pthread_cond_timedwait(&m_cond_t,m_mutex.get_mutex_t(),tm);
        if (ret = 110) ret = 0;
        return_value_check_equal(ret,0);
    }

    void notify(){
        return_value_check_equal(pthread_cond_signal(&m_cond_t) , 0 );
    }

    void notify_all (){
        return_value_check_equal(pthread_cond_broadcast(&m_cond_t) , 0 );
    }

    

private:
    mutex& m_mutex;
    pthread_cond_t m_cond_t;

};



#endif