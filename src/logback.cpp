#include "logback.hpp"

// /*
// tmp function
// */
// #include <stdio.h>
// FILE* fp = fopen("data.txt","wb");
// void test_write(const char* data, int len) {
//     fwrite(data, 1, len, fp);
//     fflush(fp);
// }


struct timespec logback::wait_time = {2,0};  

logback::logback() : m_mutex(), m_thread([this](){this->working();}), cond(m_mutex){
    
    log_buf_size = log_buf_t::get_buffer_size();
    // allocate at least one buffer for write
    m_free_bufs.push(log_buf_uq_t(new log_buf_t))  ;
    m_running = true;
    m_thread.start();
} 

logback::~logback(){
    if(m_running){
        stop();
    }
}

void logback::stop() {
    m_running = false;
    cond.notify_all();
    m_thread.join();
}


bool logback::feed(const char* data, int len){
    if (len > log_buf_size) {
        return false; // exceed largest space size
    }
    
    lock_guard lk(m_mutex);

    //prepare writting space
    if (m_free_bufs.empty()){
        m_free_bufs.push(log_buf_uq_t(new log_buf_t));
    } else if (len > m_free_bufs.front()->get_remain_space()){
        // transfer to write bufs and notify
        m_full_bufs.push(std::move(m_free_bufs.front()));  
        cond.notify();
        m_free_bufs.pop();
       if(m_free_bufs.empty()){
           m_free_bufs.push(log_buf_uq_t(new log_buf_t));
       }
    }

    m_free_bufs.front()->write(data, len);

    return true;
}

void logback::working() { // single thread
    std::queue<log_buf_uq_t> tmp_full_buffs;

    while(m_running) {
        lock_guard lk(m_mutex);
        if (m_full_bufs.empty() && m_running) {
            cond.time_wait(&wait_time);
        }

        if (m_full_bufs.empty()){
            if (m_free_bufs.front()->get_current_position() == 0){
                continue; 
            }
            else{
                m_full_bufs.push(std::move(m_free_bufs.front()));
                m_free_bufs.pop();
            }
        }

        while (!m_full_bufs.empty()){
            log_buf_uq_t tmp = std::move(m_full_bufs.front());
            m_full_bufs.pop();
            
            //here just unlock mutex to asynchronously write data, otherwise the 
            // feed function will be blocked by the following writing operation
            lk.unlock();
            //now write buffer to file
            m_logfile.write(tmp->get_data(), tmp->get_current_position());
            tmp->reset();

            lk.lock();
            // if no pure free buffer in m_free_bufs, then resue the buf pointer by tmp
            if(m_free_bufs.empty() ||  m_free_bufs.back()->get_current_position() != 0){ 
                m_free_bufs.push(std::move(tmp));
            } 
            //otherwise, tmp will be destruct
        }


    }
}




bool logback::is_running(){
    return m_running;
    
}