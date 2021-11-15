#include "fileutils.hpp"



std::string logfile::m_folder_path = "./log";


logfile::logfile() : m_fp(NULL) {
    m_filename = get_custom_time("%Y-%m-%d") ;
    update_file();
}

logfile::~logfile() {
    fclose(m_fp);
}


void logfile::update_file() {
    std::string file_path = m_folder_path + "/" + m_filename + ".log";
    m_fp = fopen(file_path.c_str(),"a+");
}
 
void logfile::try_update_file() {
    std::string new_name = get_custom_time("%Y-%m-%d") ;
    if (new_name != m_filename) { //create and open a new file
        update_file();
    }
}


void logfile::write(const char* data, int len){
    try_update_file();
    fwrite(data,1,len, m_fp);
    fflush(m_fp);
}