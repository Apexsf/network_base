#include "thread.hpp"
#include "mutex.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <queue>
#include <random>
#include <memory>

using std::cout;
using std::endl;

void print(){
    cout << "hello world" << endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
}

void test_basic(){
    thread th(&print);
    cout << "is running : " << th.is_running() << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    th.start(); 
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "is running : " << th.is_running() << endl;
    th.join();
    cout << "is running : " << th.is_running() << endl;
}



void test_multiple_thread(){
 
    auto F = [] () { //without mutex
        static long cnt;
        auto start = std::chrono::system_clock::now();
        for(int i = 0; i < 4000; i++){
            cnt++;
            cout << cnt << endl;
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> dur = end-start;
        cout << "duration : " << dur.count() << "  cnt : " << cnt << endl;
    };


    mutex mt;
    auto F2 = [&mt] () { //with mutex
        static long cnt;
        auto start = std::chrono::system_clock::now();
        for(int i = 0; i < 4000; i++){
            lock_guard lk(mt);
            cnt++;
            cout << cnt << endl;
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> dur = end-start;
        cout << "duration : " << dur.count() << "  cnt : " << cnt << endl;
    
    };

   std::vector<std::unique_ptr<thread>> ths;
    for(int i = 0; i < 100; i++){
       ths.emplace_back(std::unique_ptr<thread>(new thread(F)));
    }
    for(auto& t_p : ths){
        t_p->start();
    }
    ths.clear();
    std::this_thread::sleep_for(std::chrono::seconds(2)); // check for last cout


    for(int i = 0; i < 100; i++){
       ths.emplace_back(std::unique_ptr<thread>(new thread(F2)));
    }
    for(auto& t_p : ths){
        t_p->start();
    }

}



int main(){
    // test_basic();
    test_multiple_thread();
}