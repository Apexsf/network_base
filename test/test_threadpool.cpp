#include "threadpool.hpp"
#include <iostream>
#include <chrono>
#include <thread>

void test_basic () {
    threadpool tp;
    tp.submit([](){std::cout << "hello threadpool" << std::endl;});
    auto add = [] (int x, int y) ->int {return x+y;};
    std::future<int> res = tp.submit(add, 4,5);
    std::cout << "added num is : " << res.get() << std::endl;
}

void test_constanc_submit() {
    threadpool tp;
    for(int i = 0; i < 10000; i++) {
        tp.submit([i](){std::cout << "task : " << i << std::endl;});
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto add = [] (int x, int y) ->int {return x+y;};
    for (int i = 0; i < 10000; i++) {
        std::future<int> res = tp.submit(add, i,i+1);
        std::cout << "res : " << res.get() << std::endl;
    }
}


int main() {
    test_basic();
    test_constanc_submit();
}