#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "eventloop.hpp"
#include "inetaddr.hpp"
#include "socket.hpp"


class listener {
    using read_func = std::function<void()>;

    listener(const inet_address& addr, eventloop* loop);
    void init();

    void set_read_cb (read_func cb) {
        if(cb){       
            m_read_cb = std::move(cb);
        }
    }

    void default_read_cb();

private:
    // void on_read();

    /**
     * @brief default read event callback, mainly used to accept new connection triggered by epoll and spawn a connection object to handle it. 
     * 
     */
    listening_socket m_socket;    
    iohandler m_handler;
    read_func m_read_cb;
};




class tcp_server {
    static void default_listen_read_cb();

private:
    listener m_listener;
    eventloop* m_loop;
    
};



#endif















