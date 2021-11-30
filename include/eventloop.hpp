#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "iohandler.hpp"
#include "poller.hpp"
#include "timer.hpp"
#include <memory>

class eventloop{
public:
    eventloop();
    void update(iohandler*);
    void loop();
    void timer_seq_read_cb();

private:
    epoller* m_poller;
    std::unique_ptr<timer_sequence> m_timer_seq;
    
};



#endif