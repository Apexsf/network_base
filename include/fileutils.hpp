#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <stdio.h>
#include "utils.hpp"

class logfile {

public:
    logfile();
    ~logfile();
    void write(const char* data, int len);
    void update_file();
    void try_update_file();

private:

    std::string m_filename;
    static std::string m_folder_path;
    FILE* m_fp;

};




#endif