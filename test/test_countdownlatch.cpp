#include "CountDownLatch.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;


CountDownLatch cdl(17);

void run(){
    cdl.wait();
    cout << "runing" << endl;
}

void count_down(){
    cdl.decrement();
}

int main(){
    std::thread runthread(run);

    std::vector<std::thread> ths;
    for(int i = 0; i < 8; i++){
        ths.emplace_back(std::thread(count_down));
    }   

    std::this_thread::sleep_for(std::chrono::seconds(2));


    for(int i = 0; i < 8; i++){
        ths.emplace_back(std::thread(count_down));
    }   

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    cout << "count down remains only 1" << endl;
    count_down();
    cout << "now running" << endl;

    runthread.join();
    for(auto& t : ths){
        t.join();
    }   

    
}