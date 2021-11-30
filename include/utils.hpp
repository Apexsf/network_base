#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <sys/stat.h>

#define LOCALTIME
// #define GMTTIME

std::string get_current_time();

std::string get_custom_time(const char* format);

bool check_for_file_exists(const char* file_path);

inline void return_value_check_equal(int value, int target) {
    assert(value == target);
}

inline void return_value_check_less(int value, int target) {
    assert (value < target);
}

inline void return_value_check_more(int value, int target) {
    assert(value > target);
}



#endif