#ifndef CONNECTION
#define CONNECTION

#include "eventloop.hpp"
/**
 * @brief connection is mainly used to handle a single tcp connection, hence it is mostly
 * created upon a new tcp connection coming in tcp_server, spawning connection to handle the 
 * following write and read event.
 * 
 */
class connection {
public:
    using on_conn_read = std::function<void()> ;
    using on_conn_write = std::function<void()>;
    connection(int fd, eventloop* loop);
    
    connection(const connection&) = delete;
    connection& operator= (const connection&) = delete;

private:
    iohandler m_handler;
    int m_fd;
};

#endif