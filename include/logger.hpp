#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <sstream>
#include <iostream>
#include <chrono>
#include "buffer.hpp"
#include <utils.hpp>


class logger {   // front end logger
public:
    enum LOG_LEVEL {
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    logger(LOG_LEVEL level = LOG_LEVEL::INFO);
    ~logger();

    template<typename T>
    logger& operator << (T&& data);

    static void setup_output_func(std::function<void (const char*, int)> func);

private:
    std::stringstream ss;
};



template<typename T>
logger& logger::operator << (T&& data) {
    ss << std::forward<T>(data);
    return *this;
}

#define LOG_TRACE (logger(logger::LOG_LEVEL::TRACE) << __FILE__ << ":" << __LINE__ << "  ")
#define LOG_DEBUG (logger(logger::LOG_LEVEL::DEBUG) << __FILE__ << ":" << __LINE__ << "  ")
#define LOG_INFO (logger(logger::LOG_LEVEL::INFO) << __FILE__ << ":" << __LINE__ << "  ")
#define LOG_WARN (logger(logger::LOG_LEVEL::WARN) << __FILE__ << ":" << __LINE__ << "  ")
#define LOG_ERROR (logger(logger::LOG_LEVEL::ERROR) << __FILE__ << ":" << __LINE__ << "  ")
#define LOG_FATAL (logger(logger::LOG_LEVEL::FATAL) << __FILE__ << ":" << __LINE__ << "  ")



#endif