#include "mutex.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using std::cout;
using std::endl;

std::vector<int> test_vec;

mutex m1;
long k_cnt = 1000*100*100;


void push_without_lock(){
    for( int i = 0 ; i < k_cnt; i ++){
        test_vec.push_back(i);   
    }
}


void push_with_lock (){
    lock_guard lock(m1);
    for( int i = 0 ; i < k_cnt; i ++){
        test_vec.push_back(i);   
    }
}

int return_value_check(int x) {
    return x;
}

void test_lock_guard_scope(){
    {
        lock_guard lk(m1);
    }

    int i = 10;
    while (--i) {
        lock_guard lk(m1);
        if (i % 2){
            continue;
        }
    }


}


int main(){
    test_lock_guard_scope();

    long num_thread = 8;
    test_vec.reserve(num_thread * k_cnt); 

    mutex m;
    cout <<"mutex t : " <<  m.get_mutex_t() << endl;
    cout << "size of mutex t " << sizeof(m.get_mutex_t()) << endl;
    cout << "size of mutex : " << sizeof(m) << endl;
    
    test_vec.clear();

    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    push_without_lock();
    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_elapsed = end-start;
    cout << "elapsed time without lock: " << time_elapsed.count() << endl;

    test_vec.clear();

        
    start = std::chrono::steady_clock::now();
    push_with_lock();
    end = std::chrono::steady_clock::now();
    time_elapsed = end-start;
    cout << "elapsed time with lock: " << time_elapsed.count() << endl;

    test_vec.clear();

    std::vector<std::thread> ths;
    start = std::chrono::steady_clock::now();
    for(int i = 0; i < num_thread; i++){
       

        ths.emplace_back(std::thread(push_without_lock));
    }

    for(int i =0; i < num_thread; i++) {

        ths[i].join();
    }

    end = std::chrono::steady_clock::now();
    time_elapsed = end-start;
    cout << "elapsed time without lock using "<< num_thread << " threads : " << time_elapsed.count() << endl;
    
    cout << "size of n without lock: " << test_vec.size() << endl;




    test_vec.clear();
    ths.clear();
    start = std::chrono::steady_clock::now();
    for(int i = 0; i < num_thread; i++){
       

        ths.emplace_back(std::thread(push_with_lock));
    }

    for(int i =0; i < num_thread; i++) {

        ths[i].join();
    }
    
    end = std::chrono::steady_clock::now();
    time_elapsed = end-start;
    cout << "elapsed time with lock using "<< num_thread << " threads : " << time_elapsed.count() << endl;
    
    cout << "size of n with lock: " << test_vec.size() << endl;




   
    return_value_check_equal(return_value_check(3), 3);



}