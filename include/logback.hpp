#ifndef LOCBACK
#define LOCBACK


#include "mutex.hpp"
#include "buffer.hpp"
#include "condition.hpp"
#include "thread.hpp"
#include "fileutils.hpp"
#include <memory>
#include <queue>


class logback{  // thread safe, buffered, timed backend to write log 

public:
    using log_buf_t = buffer<8192>;
    using log_buf_uq_t  = std::unique_ptr<log_buf_t>;
    static struct timespec wait_time ; //try to output buffer every 2 second regardless full or not
    
    logback();

    ~logback();
    
    bool feed(const char* data, int len);

    bool is_running();

    void working(); // single working thread

    void stop();


private:
    bool m_running;
    int log_buf_size;
    thread m_thread;
    mutex m_mutex;
    condition cond;
    logfile m_logfile;

    std::queue<log_buf_uq_t> m_free_bufs;
    std::queue<log_buf_uq_t> m_full_bufs;

};


#endif