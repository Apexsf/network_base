#ifndef COUNT_DOWN_LATCH_H
#define COUTN_DOWN_LATCH_H

#include <pthread.h>
#include <mutex.hpp>
#include <condition.hpp>


class CountDownLatch {
public:
    CountDownLatch(int cnt) : m_cnt(cnt), m_mutex(), m_cond(m_mutex){

    }

    CountDownLatch(const CountDownLatch&) = delete;
    CountDownLatch& operator = (const CountDownLatch&) = delete;

    int get_cnt() {
        return m_cnt; 
    }

    void wait() {
        lock_guard lock(m_cond.get_mutex());
        m_cond.wait([this](){return this->m_cnt;});
    }


    void decrement() {
        lock_guard lock(m_cond.get_mutex());
        --m_cnt;
        if (m_cnt == 0) {
            m_cond.notify_all();
        }
    }


private:
    int m_cnt;
    mutex m_mutex;
    condition m_cond;
};



#endif