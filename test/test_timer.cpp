#include "eventloop.hpp"
#include <iostream>
#include <string>
#include <thread>

eventloop* loop_p;

void print(std::string str) {
    std::cout << str << std::endl;
}

void add_timer_task () {
    time_point tp = time_point::time_point_now();
    tp.advance(time_duration(2));
    loop_p->run_at(tp, [](){print("test run at 2");});
    tp.advance(time_duration(1.5));
    loop_p->run_at(tp, [](){print("test run at 1.5");});
    tp.advance(time_duration(0.5));
    loop_p->run_at(tp, [](){print("test run at 0.5");});


    time_duration td(3);
    loop_p->run_after(td, [](){print("test run after 3");});
    time_duration td2(4.5);
    loop_p->run_after(td2, [](){print("test run after 4.5");});

    time_point tp2 = time_point::time_point_now();
    time_duration inval = time_duration(2.5);
    loop_p->run_every(tp2, inval, [](){print("test run every");});
}



int main() {
    loop_p = new eventloop();       
    add_timer_task();
    loop_p->loop();    
}