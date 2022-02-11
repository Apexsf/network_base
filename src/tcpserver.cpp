#include "tcpserver.hpp"
#include "socket.hpp"


listener::listener(const inet_address& addr, eventloop* loop):
m_socket(addr),m_handler(m_socket.get_sock_fd(), loop),m_read_cb([this](){this->default_read_cb();}){

}

void listener::init() {
    m_handler.set_ev_read();
    m_handler.set_read_cb([this](){(this->m_read_cb)();}); 
    m_handler.add_to_poller();
}



void listener::default_read_cb() {
    inet_address conn_addr(true);
    int conn_fd = m_socket.accept(conn_addr);
}