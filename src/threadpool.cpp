#include "threadpool.hpp"

threadpool::threadpool(int size) : m_size(size), m_running(true),m_mut(),m_cond(m_mut){
    for(int i = 0; i < m_size; i++) {
        m_ths.emplace_back(
            std::make_unique<thread>([this](){   
                while(this->m_running){
                    lock_guard lk(m_mut);
                    m_cond.wait([this]()->bool{return this->m_que.empty() && this->m_running;});
                    if (!this->m_que.empty()){
                        func_type func = std::move( m_que.front() );
                        m_que.pop();
                        lk.unlock();
                        func();
                    }

                }
            })
        );
        m_ths.back()->start();
    }
}


threadpool::~threadpool() {
    stop();
}

void threadpool::stop() {
    m_running = false;
    m_cond.notify_all();
    for(int i = 0; i < m_size; i++){
        m_ths[i]->join();
    }
}