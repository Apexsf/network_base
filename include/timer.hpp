#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <chrono>
#include <queue>
#include <memory>
#include <set>
#include <sys/timerfd.h>

#include <time.h>
#include <string.h>
#include "iohandler.hpp"

class time_duration; // forward declaration
/**
 * @brief TODO: encapsulate with unix timespec directly to avoid conversion between std::chrono and unix timespec for timerfd_XXX function.
 * 
 */
class time_point {
public:
    using timer_clock = std::chrono::system_clock;
    using timer_point = std::chrono::time_point<timer_clock>;
    using rep = timer_point::rep;

    time_point() = default;
    time_point(const timer_point& tp);
    rep get_count();
    const timer_point& get_time() const;
    void set_now();
    static time_point time_point_now() {
        time_point tp;
        tp.set_now();
        return tp;
    }

    void advance(double seconds);
    void advance(const time_duration& td);

    friend bool operator < (const time_point& left, const time_point& right);
  
private:
    timer_point m_time;
};


class time_duration {
public:
    using nanoseconds = std::chrono::nanoseconds;

    /**
     * @brief Construct a new time duration object from double seconds.
     * 
     * @param seconds The time duration of seconds encoded by double. The most precise time segment is nanosecond (1e-9 second). 
     */
    time_duration(double seconds);

    /**
     * @brief Construct a time_duration from std::chrono::nanoseconds directly.
     * 
     */
    explicit time_duration(const nanoseconds&);

    static time_duration zero_time_duration() {
        return time_duration(nanoseconds(0));
    }

    bool is_zero() {
        return m_dur == nanoseconds(0);
    }

    const nanoseconds& get_chrono_nanosecond() const {
        return m_dur;
    }

private:
    nanoseconds m_dur;
};


struct time_util {
    using nanoseconds = std::chrono::nanoseconds;
    static time_duration timespec_2_timeduration(const struct timespec&);
    static struct timespec timeduration_2_timespec(const time_duration&);
    static time_point timespec_2_timepoint (const struct timespec&);
    static struct timespec timepoint_2_timespec(const time_point&);
};

class timer_task {
public:
    using timer_func = std::function<void()>;
    timer_task(const timer_func&, const time_point&, const time_duration& interval);
    const time_point& get_time_point() const;
    void reset_task(const timer_func&);
    void reset_time_point(const time_point&);
    void call();


    struct timer_task_less {
        bool operator() (const timer_task& left, const timer_task& right) const{
            return left.m_time_point < right.m_time_point;
        }
    };

private:
    timer_func m_task;
    time_point m_time_point;
    time_duration m_interval;
};


/**
 * @brief timer_sequence serves as the backend for timer functionality of eventloop.
 * It is the responsibiliess of eventloop to handle the expired timer task, while 
 * timer_sequence only manages the timer task set and provides the get_expired_task  
 * interface for the further usage of eventloop.
 * Besides, note that tiemr_sequence is invisible to user.
 * 
 * TODO: change the element store in the internal multiset to raw pointer or 
 * smart point to avoid frequent copy
 *   
 */
class timer_sequence{
public:
    using read_cb = std::function<void(void)>;
    timer_sequence(eventloop*, int);

    void set_on_read_handler(const read_cb& cb) {  
        m_read_cb = cb;
    }

    /**
     * @brief Note : whether the timer_task is periodic, it is registered to timerfd
     * as one-shot function (the new_value.it_interval pass to timerfd_settime is always set zero), since the periodic functionality provided by timerfd conflicts with this insert method, saying that makes it hard to calculate the nearest time point when mixing the one-shot and periodic timer inside insert. Here we choose to handle the periodic timer_task by encoding the time interval with m_interval of timer_task (m_interval of zero means not a periodic timer task). It is the responsibilities of outside (the method execute the timer task) to reregister the periodic timer task via this method.
     * 
     * 
     */
    void insert(const timer_task&);



    /**
     * @brief prefered insert method, which can take advantage of multiset emplace
     * method to avoid a extra copy
     * 
     * @param func, @param tp, @param td, are the arguments of time_task constructor
     */
    void insert(const timer_task::timer_func& func, const time_point& tp,
    const time_duration& td);


    /**
     * @brief Get the timer_tasks of which the expired time is earlier than tp 
     * 
     * @return std::vector<timer_task>.
     */
    std::vector<timer_task> remove_and_get_expired_task(const time_point& tp);

    void reset_earliest_time();
private:
    using timer_task_set = std::multiset<timer_task, timer_task::timer_task_less>;
  
    eventloop* m_loop;
    int m_timer_fd; 
    iohandler m_timerfd_handler;
    timer_task_set m_timer_task_multiset; // alternative structure is priority queue, may be testing later
    read_cb m_read_cb;

};


#endif