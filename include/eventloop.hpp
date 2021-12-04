#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "iohandler.hpp"
#include "poller.hpp"
#include "timer.hpp"
#include <memory>

class eventloop{
public:
    using timer_func = timer_task::timer_func;
    eventloop();
    void update(iohandler*);
    void loop();
    void run_at(const time_point&, const timer_func&);
    void run_after(const time_duration&, const timer_func&);

    /**
     * @brief run every 
     * 
     */
    void run_every(const time_point& , const time_duration&, const timer_func&);
    


private:
    void timer_seq_read_cb();
    epoller* m_poller;
    std::unique_ptr<timer_sequence> m_timer_seq;
};



#endif