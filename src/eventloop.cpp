#include "eventloop.hpp"



eventloop::eventloop():m_poller(epoller::get_epoller())
    // m_timer_seq(std::make_unique<timer_sequence>())
 {
     int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
     return_value_check_more(fd, -1);
     m_timer_seq.reset(new timer_sequence(this, fd));

}

void eventloop::update(iohandler* handler) {
    m_poller->update(handler);
}


void eventloop::loop() {
    std::vector<iohandler*> polling_handlers;
    while(true) {
        polling_handlers.clear();
        m_poller->poll(polling_handlers,2000);
        for (iohandler* handler : polling_handlers) {
            handler->handle_event();
        }
    }

}

void eventloop::timer_seq_read_cb() {
    std::vector<timer_task> expired_tasks = m_timer_seq->remove_and_get_expired_task(time_point::time_point_now());
    for (timer_task& tt : expired_tasks) {
        tt.call();
    }
    m_timer_seq->reset_earliest_time();
}


void eventloop::run_at (const time_point& tp, const timer_func& tf) {
    m_timer_seq->insert(tf, tp, time_duration::zero_time_duration());
}

void eventloop::run_after(const time_duration& td, const timer_func& tf) {
    time_point tp = time_point::time_point_now();
    tp.advance(td);
    run_at(tp, tf);
}

void eventloop::run_every(const time_point& tp, const time_duration& td, const timer_func& tf) {
    m_timer_seq->insert(tf,tp,td);
}


