#ifndef BUFFER_H
#define BUFFER_H

#include <string.h>

#define SMALL_BUFFER 4096
#define LARGE_BUFFER 4096 * 100


template<int BUFFSIZE>
class buffer {
public:
    buffer() : m_cur_p(0) {}
    bool write(const char* write_data, int len) {
        if (len > BUFFSIZE - m_cur_p) {
            return false;   //remaining buffer size is not sufficient
        } else {
            memcpy(m_data + m_cur_p, write_data, len) ;
            m_cur_p += len;
            return true;
        }    
    }

    static int get_buffer_size () {
        return BUFFSIZE;
    }

    int get_current_position() {
        return m_cur_p;
    }

    int get_remain_space() {
        return BUFFSIZE - m_cur_p;
    }

    const char* get_data() {
        return m_data;
    }

    void reset() {
        m_cur_p = 0;    // reset position to zero
    }


private:
    char m_data[BUFFSIZE];
    int m_cur_p;  // current position
};


using small_buffer = buffer<SMALL_BUFFER>;
using large_buffer = buffer<LARGE_BUFFER>;


#endif