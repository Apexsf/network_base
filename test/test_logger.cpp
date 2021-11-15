#include "logger.hpp"
#include "buffer.hpp"

void test_basic_log(){
    LOG_TRACE << "LOG TRACE " << "level : " << logger::LOG_LEVEL::TRACE;
    LOG_DEBUG << "LOG DEBUG " << "level : " << logger::LOG_LEVEL::DEBUG;
    LOG_INFO << "LOG INFO"<< "level : " << logger::LOG_LEVEL::INFO;
    LOG_WARN << "LOG WARN" << "level : " << logger::LOG_LEVEL::WARN;
    LOG_ERROR << "LOG ERROR" << "level : " << logger::LOG_LEVEL::ERROR;
    LOG_FATAL << "LOG FATAL" << "level : " << logger::LOG_LEVEL::FATAL;
}


void test_consecutive_log(){
    using namespace std::chrono;
    system_clock::time_point start  = system_clock::now();
    for(int i = 0; i < 100000 / 6; i++){
        test_basic_log();
    }
    system_clock::time_point end = system_clock::now();

    duration<double> dur = end - start;
    std::cout << "running time : " << dur.count()  << "seconds" << std::endl;
}




void test_consecutive_log_withbuf(){
    small_buffer sb;
    auto custom_write =  [&sb](const char* data, int len) {
            while (!sb.write(data, len)){
        // std::cout << sb.get_data() << std::endl;
                sb.reset();
        
                 }
        };

    logger::setup_output_func(custom_write);
    test_consecutive_log();

}


int main(){
    // logger log;
    // log << "haha : " << 1 ;
    test_basic_log();
    // test_consecutive_log();
    test_consecutive_log_withbuf();
}