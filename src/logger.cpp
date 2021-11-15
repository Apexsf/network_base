#include "logger.hpp"


char const * log_level_list[] = {"TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};



void default_output_func(const char* data, int len) {
        std::cout << data << std::endl;
}

std::function<void(const char*, int)> output_func = default_output_func;


void logger::setup_output_func(std::function<void (const char*, int)> func){
    output_func = func;
}


logger::logger(LOG_LEVEL level) {
    ss << log_level_list[level] << " " << get_current_time() << " " ;
}

logger :: ~logger() {
    ss << "\n";
    ss.seekg(0, std::ios::end);
    output_func(ss.str().c_str(), ss.tellg());
}

