#include "timer.hpp"

#include "utils.hpp"

bool operator < (const time_point& left, const time_point& right){
    return left.m_time < right.m_time;
}

time_point::time_point(const time_point::timer_point& tp):m_time(tp) {}

void time_point::set_now() {
    m_time = timer_clock :: now();    
}

void time_point::advance(double seconds) {
    m_time  += std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::duration<double>(seconds) );
}

const time_point::timer_point& time_point::get_time() const {
    return m_time;
}

time_point::rep time_point::get_count() {
    return m_time.time_since_epoch().count();
}


time_duration::time_duration(double second) {
    m_dur = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::duration<double>(second)
    );
}

time_duration::time_duration(const nanoseconds& nscs) : m_dur(nscs) {} 


time_duration time_util::timespec_2_timeduration(const struct timespec& ts) {
    nanoseconds nscs(ts.tv_sec * 1000000000 + ts.tv_nsec);
    return time_duration(nscs);
}

struct timespec time_util::timeduration_2_timespec(const time_duration& td) {
    const nanoseconds& nscs = td.get_chrono_nanosecond();
    std::chrono::seconds scs = std::chrono::duration_cast<std::chrono::seconds> (nscs);
    std::chrono::nanoseconds frac_nscs = nscs-scs;
    return timespec{scs.count(), frac_nscs.count()};
}

time_point time_util::timespec_2_timepoint(const struct timespec& ts) {
    time_point::timer_point tp (
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
        timespec_2_timeduration(ts).get_chrono_nanosecond()
    )
    );
    return time_point(tp);
}

struct timespec time_util::timepoint_2_timespec (const time_point& tp) {
    std::chrono::time_point<time_point::timer_clock, std::chrono::seconds> secs = std::chrono::time_point_cast<std::chrono::seconds> (tp.get_time());
    std::chrono::nanoseconds nsecs = std::chrono::time_point_cast<std::chrono::nanoseconds> (tp.get_time()) - std::chrono::time_point_cast<std::chrono::nanoseconds> (secs);
    return timespec{secs.time_since_epoch().count(), nsecs.count()};
}


timer_task::timer_task(const timer_func& task, const time_point& point, const time_duration& interval) 
: m_task(task),m_time_point(point), m_interval(interval)
{

} 

const time_point& timer_task::get_time_point() const {
    return m_time_point;
}

void timer_task::reset_task(const timer_func& task){
    m_task = task;
}

void timer_task:: reset_time_point(const time_point& timer) {
    m_time_point = timer;
}

timer_sequence::timer_sequence(eventloop* loop, int timer_fd):
m_loop(loop), m_timer_fd(timer_fd), m_timerfd_handler(timer_fd, loop) {

}


void timer_sequence::insert(const timer_task& task) {
    m_timer_task_multiset.insert(task);
    timer_task_set::iterator first_it = m_timer_task_multiset.begin();
    if (first_it != m_timer_task_multiset.end()){
        const time_point& expired_point = first_it->get_time_point();
        struct timespec ts = time_util::timepoint_2_timespec(expired_point);

        struct itimerspec itimer_new;
        struct timespec zero_ts;
        memset(&zero_ts, 0, sizeof(zero_ts));
        itimer_new.it_interval = zero_ts;
        itimer_new.it_value = ts;
        timerfd_settime(m_timer_fd,TFD_TIMER_ABSTIME,&itimer_new,nullptr);
    }
}

std::vector<timer_task> timer_sequence::get_expired_task(const time_point& tp) {
    // be careful the pitfall here, the m_task of tmp is just set to nullptr for the following
    // upper_bound search , we should never execute m_task of tmp.
    timer_task tmp(nullptr, tp);
    timer_task_set::iterator it = m_timer_task_multiset.upper_bound(tmp);

    std::vector<timer_task> vec;
    std::copy(m_timer_task_multiset.begin(), it, std::back_inserter(vec));
    return vec;
}

