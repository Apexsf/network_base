#include "logback.hpp"
#include "logger.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void test_basic() {
    logback* lb = logback::get_logback();
    logger::setup_output_func([&lb](const char* data, int len){
        lb->feed(data, len);
    });

    for( int i = 0; i < 10000; i++){
            LOG_TRACE << "LOG TRACE " << "level : " << logger::LOG_LEVEL::TRACE ;
            LOG_DEBUG << "LOG DEBUG " << "level : " << logger::LOG_LEVEL::DEBUG ;
            LOG_INFO << "LOG INFO"<< "level : " << logger::LOG_LEVEL::INFO;
            LOG_WARN << "LOG WARN" << "level : " << logger::LOG_LEVEL::WARN;
            LOG_ERROR << "LOG ERROR" << "level : " << logger::LOG_LEVEL::ERROR;
            LOG_FATAL << "LOG FATAL" << "level : " << logger::LOG_LEVEL::FATAL;
    }
}


void test_multithread_core(){
    static logback* lb = logback::get_logback();
    logger::setup_output_func([](const char* data, int len){
        lb->feed(data, len);
    });

    for( int i = 0; i < 50000; i++){
            LOG_TRACE << "LOG TRACE " << "level : " << logger::LOG_LEVEL::TRACE ;
            LOG_DEBUG << "LOG DEBUG " << "level : " << logger::LOG_LEVEL::DEBUG ;
            LOG_INFO << "LOG INFO"<< "level : " << logger::LOG_LEVEL::INFO;
            LOG_WARN << "LOG WARN" << "level : " << logger::LOG_LEVEL::WARN;
            LOG_ERROR << "LOG ERROR" << "level : " << logger::LOG_LEVEL::ERROR;
            LOG_FATAL << "LOG FATAL" << "level : " << logger::LOG_LEVEL::FATAL;
    }

}

void test_multithread_log() {
    // test_basic();
    using namespace std::chrono;
    system_clock::time_point start  = system_clock::now();
    {
        std::vector<std::unique_ptr<thread>> ths(20);
        for(auto& t : ths) {
            t.reset(new thread(test_multithread_core));
            t->start();
        }
    }
    system_clock::time_point end = system_clock::now();
    duration<double> dur = end - start;
    std::cout << "running time : " << dur.count()  << "seconds" << std::endl;
}

void test_slow_log() {
        logback* lb = logback::get_logback();
        logger::setup_output_func([&lb](const char* data, int len){
            lb->feed(data, len);
        });
        LOG_TRACE << "LOG TRACE " << "level : " << logger::LOG_LEVEL::TRACE ;
        std::this_thread::sleep_for(std::chrono::seconds(3) ) ;

        LOG_DEBUG << "LOG DEBUG " << "level : " << logger::LOG_LEVEL::DEBUG ;
        std::this_thread::sleep_for(std::chrono::seconds(3) ) ;

        LOG_INFO << "LOG INFO"<< "level : " << logger::LOG_LEVEL::INFO;
        std::this_thread::sleep_for(std::chrono::seconds(3) ) ;

        LOG_WARN << "LOG WARN" << "level : " << logger::LOG_LEVEL::WARN;
        std::this_thread::sleep_for(std::chrono::seconds(3) ) ;    

        LOG_ERROR << "LOG ERROR" << "level : " << logger::LOG_LEVEL::ERROR;
        std::this_thread::sleep_for(std::chrono::seconds(3) ) ;

        LOG_FATAL << "LOG FATAL" << "level : " << logger::LOG_LEVEL::FATAL;
        std::this_thread::sleep_for(std::chrono::seconds(3) ) ;
}


int main() {
    // test_basic();
    test_multithread_log();
    // test_slow_log();
}