#include "condition.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <queue>
#include <random>

using std::cout;
using std::endl;


mutex mt;
condition cond(mt);
std::queue<int> que;
bool running;
    

void wait(){
    lock_guard lock(cond.get_mutex());
    cond.wait();    
    cout << "this thread id : " << std::this_thread::get_id() << endl;
}

void test_wait_nofity(){
    std::thread t(wait);  
    std::this_thread::sleep_for(std::chrono::seconds(4));
    cond.notify();  
    t.join(); 
}

void test_wait_nofity_all(){
    std::vector<std::thread> ths;
    for(int i = 0; i < 8; i ++){
        ths.emplace_back(std::thread(wait));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    cond.notify();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cond.notify();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cond.notify();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cond.notify_all();


    for(int i = 0; i < 8; i ++){
        ths[i].join();
    }
}

void produce() {
    while(running){
        lock_guard lock(mutex);
        int speed = std::rand() % 500 + 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        que.push(1);
        cond.notify();
    }
}

void consume() {
    while(running) {
        lock_guard lock(cond.get_mutex());
        cond.wait([]()->bool{return que.empty();});
        int speed = std::rand() % 500 + 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        que.pop();
        cout << que.size() << endl;
    }
}


void test_produce_consume_pattern(){  
    running = true;  
    int produce_thread_num = 8;
    int consume_thread_num = 8;
    std::vector<std::thread> produce_ths;
    std::vector<std::thread> consume_ths;

    int during_seconds = 10;

    for(int i = 0; i < produce_thread_num; i++) {
        produce_ths.emplace_back(std::thread(produce));
    }
    for(int i = 0; i < consume_thread_num; i++){
        consume_ths.emplace_back(std::thread(consume));
    }

    std::this_thread::sleep_for(std::chrono::seconds(during_seconds));
    
    running = false;
    cond.notify_all();
    for(std::thread& t : produce_ths) {
        t.join();
    }
    for(std::thread& t : consume_ths) {
        t.join();
    }
}



int main(){
    test_produce_consume_pattern();
    test_wait_nofity();
    test_wait_nofity_all();
}