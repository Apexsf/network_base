#include "timer.hpp"
#include <iostream>
#include <thread>
#include <assert.h>

void test_timer_util() {
    time_duration t2(4);

    time_point tp1;
    tp1.set_now();
    struct timespec ts1 = time_util::timepoint_2_timespec(tp1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    time_point tp1_exchange = time_util::timespec_2_timepoint(ts1);

    assert(tp1.get_time() == tp1_exchange.get_time());
    
    std::this_thread::sleep_for(std::chrono::seconds(3));

    struct timespec ts2;
    clock_gettime(CLOCK_REALTIME, &ts2);
    time_point tp2 = time_util::timespec_2_timepoint(ts2);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    struct timespec ts2_exchange = time_util::timepoint_2_timespec(tp2);

    assert(ts2.tv_sec == ts2_exchange.tv_sec);
    assert(ts2.tv_nsec == ts2_exchange.tv_nsec);

    std::cout << "test time util done" << std::endl;

   
}



int main() {
    test_timer_util();
}