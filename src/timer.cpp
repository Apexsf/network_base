#include "timer.hpp"


time_point::time_point():m_time(timer_clock::now()) {

}

void time_point::set_now() {
    m_time = timer_clock :: now();    
}

void time_point::advance(double seconds) {
    m_time  += std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::duration<double>(seconds) );
}

time_point::timer_point time_point::get_time() {
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


timer_task::timer_task(const timer_func& task) : m_task(task),m_timer(){

} 

void timer_task::reset_task(const timer_func& task){
    m_task = task;
}

void timer_task:: reset_timer(const time_point& timer) {
    m_timer = timer;
}





