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
#include <string.h>
#include <fcntl.h>

#define LOCALTIME
// #define GMTTIME

std::string get_current_time();

std::string get_custom_time(const char* format);

bool check_for_file_exists(const char* file_path);

inline void return_value_check_equal(int value, int target) {
    assert(value == target);
}

inline void return_value_check_unequal (int value, int target) {
    assert(value != target);
}

inline void return_value_check_less(int value, int target) {
    assert (value < target);
}

inline void return_value_check_more(int value, int target) {
    assert(value > target);
}

inline std::string get_errno_name (int err) {
    return strerror(err);
}

inline void mem_zero(void* p, size_t n) {
    memset(p, 0, n);
}

inline void set_fd_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return_value_check_unequal(flags, -1);
    int ret =  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return_value_check_unequal(ret, -1);
}



#endif