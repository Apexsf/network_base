#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <assert.h>
#include "utils.hpp"

#define ASSERT_ZERO_RETURN(call) ({ __typeof__(call) return_num  = call; \
assert(return_num == 0); \
(void) return_num; })


class mutex {
public:

    mutex(const mutex&) = delete;

    mutex& operator= (const mutex&) = delete;

    mutex() {
       return_value_check_equal(::pthread_mutex_init(&m_mutex_t, NULL) , 0);
    }

    ~mutex(){
        return_value_check_equal( ::pthread_mutex_destroy(&m_mutex_t),0);
    }

    pthread_mutex_t* get_mutex_t() {
        return &m_mutex_t;
    }

    void lock(){
       return_value_check_equal( pthread_mutex_lock(&m_mutex_t) , 0 );
    }

    void unlock(){
        return_value_check_equal( pthread_mutex_unlock(&m_mutex_t), 0);
    }

private:
    pthread_mutex_t m_mutex_t;
};



class lock_guard {
public:
    lock_guard(mutex& mutex) : m_mutex(mutex) , m_is_lock(true){
        m_mutex.lock();
    }

    void unlock() {
        m_mutex.unlock();
        m_is_lock = false;
    }

    void lock() {
        if (!m_is_lock){
            m_mutex.lock();
            m_is_lock = true;
        } 
    }
    

    ~lock_guard() {
        if (m_is_lock) {
            m_mutex.unlock();
        } 
    }

private:
    mutex& m_mutex;
    bool m_is_lock;

};




#endif