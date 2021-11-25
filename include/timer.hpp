#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <chrono>
#include <queue>

class time_point {
public:
    using timer_clock = std::chrono::steady_clock;
    using timer_point = std::chrono::time_point<timer_clock>;
    using rep = timer_point::rep;

    time_point();
    rep get_count();
    timer_point get_time();
    void set_now();  
    void advance(double seconds);

private:
    timer_point m_time;
};


class time_duration {
public:
    using seconds = std::chrono::seconds;
    using milliseconds = std::chrono::milliseconds;
    using nanoseconds = std::chrono::nanoseconds;
    time_duration(double seconds);

private:
    nanoseconds m_dur;
};



class timer_task {
public:
    using timer_func = std::function<void()>;
    timer_task() = default;
    timer_task(const timer_func&);
    void reset_task(const timer_func&);
    void reset_timer(const time_point&);
    

private:
    timer_func m_task;
    time_point m_timer;
};



class timer_sequence{
private:
    int m_timerfd;
};


#endif