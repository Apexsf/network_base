#include "eventloop.hpp"



eventloop::eventloop():m_poller(epoller::get_epoller())
    // m_timer_seq(std::make_unique<timer_sequence>())
 {
     // FIXME: Note that we create timerfd by param CLOCK_REALTIME in order to 
     // corporate with time_point and time_duration where chrono clock type is
     // system_clock, otherwise the timer will be messy due to inconsistent
     // interpretation of the value of time_spec.(see the static methods of time_util
     // and timerfd_settime).
     // However, the better clock type is monotonic clock, I'll consider change 
     // to it later.
     int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
     return_value_check_more(fd, -1);
     m_timer_seq.reset(new timer_sequence(this, fd));
     m_timer_seq->set_on_read_handler([this](){this->timer_seq_read_cb();});
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
        if(tt.is_periodic()){
            time_point new_tp = time_point::time_point_now();
            new_tp.advance(tt.get_time_interval());
            m_timer_seq->insert(tt.get_task(),new_tp,tt.get_time_interval());
        }
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


