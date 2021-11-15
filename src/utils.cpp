#include "utils.hpp"


std::string get_current_time(){
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
   
    #if defined(LOCALTIME)
        ss << std::put_time(std::localtime(&in_time_t), "L%Y-%m-%d_%X"); 

    #elif defined(GMTTIME)
        ss << std::put_time(std::gmtime(&in_time_t), "G%Y-%m-%d_%X"); 
    #else
        #error
    #endif

    std::chrono::system_clock::duration tp = now.time_since_epoch();
    tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);

    int microseconds = tp / std::chrono::microseconds(1);   

    ss << "." << microseconds;
    return ss.str();
}



std::string get_custom_time(const char* format){
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    #if defined(LOCALTIME)
        ss << std::put_time(std::localtime(&in_time_t), format); 

    #elif defined(GMTTIME)
        ss << std::put_time(std::gmtime(&in_time_t), format); 
    #else
        #error
    #endif

    return ss.str();

}

bool check_for_file_exists(const char* file_path) {
    struct stat fileInfo;
    return stat(file_path, &fileInfo) == 0;
}