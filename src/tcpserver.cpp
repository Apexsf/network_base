#include "tcpserver.hpp"
#include "socket.hpp"


listener::listener(const inet_address& addr, eventloop* loop, 
new_conn_func connect_cb):
m_socket(addr),m_handler(m_socket.get_sock_fd(), loop),m_read_cb([this](){this->default_read_cb();}), m_new_conn_cb(connect_cb) {
    m_socket.set_non_block();
    init();
}

void listener::init() {
    m_handler.set_ev_read();
    m_handler.set_read_cb([this](){(this->m_read_cb)();}); 
    m_handler.add_to_poller();
}


void listener::default_read_cb() {
    inet_address conn_addr(true);
    int conn_fd = m_socket.accept(conn_addr);
    m_new_conn_cb(conn_fd, conn_addr);
}



tcp_server::tcp_server(eventloop* loop, const message_func& message_cb, inet_address& addr) : m_message_cb (message_cb), m_listener(addr, loop, [this](int sockid, const inet_address& addr) {this->default_listen_read_cb(sockid, addr);}
) {

}


void tcp_server::default_listen_read_cb(int sockid, const inet_address& addr) {
    std::cout << "debug, tcp_server :: default_listen_read_cb, details will be implemented later." << std::endl;
}