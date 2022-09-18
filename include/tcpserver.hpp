#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "eventloop.hpp"
#include "inetaddr.hpp"
#include "socket.hpp"
#include "buffer.hpp"


class listener {
public:
    using read_func = std::function<void()>;
    using new_conn_func = std::function<void(int sockid,  const inet_address& addr)>;

    listener(const inet_address& addr, eventloop* loop, new_conn_func connect_cb);
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
    new_conn_func m_new_conn_cb;
};


class tcp_connector {

private:
    iohandler m_handler;
    
};



class tcp_server {
public:
 

    using tcp_connector_sharep = std::shared_ptr<tcp_connector>;
    using conn_func = std::function<void(const tcp_connector_sharep&)>;
    using message_func = std::function<void(const tcp_connector_sharep&, small_buffer&)>; 
    
    /**
     * @param addr: given address of an initialized listen socket
     */
    tcp_server(eventloop* loop, const message_func& message_b, inet_address& addr);

    // being called once new connection reaches in listener
    void default_listen_read_cb(int sockid, const inet_address& addr);




private:
    listener m_listener;
    eventloop* m_loop;
    message_func m_message_cb;
    
};



#endif















