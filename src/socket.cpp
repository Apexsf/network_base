#include "socket.hpp"


general_socket::general_socket(int fd) : m_sock_fd (fd){

}

general_socket::general_socket(int domain, int type, int protocol){
    m_sock_fd = ::socket(domain, type, protocol);
    return_value_check_more(m_sock_fd,-1);
}



listening_socket::listening_socket(inet_address in_addr):
m_addr(in_addr), 
general_socket(AF_INET, SOCK_STREAM,0){ // only considering tcp now
    init();
}

listening_socket::listening_socket(uint16_t port, bool is_any, bool is_ipv4):
m_addr(port, is_any, is_ipv4),
general_socket(AF_INET, SOCK_STREAM,0) {
    init();
}

void listening_socket::init() {
    int ret;
    ret = ::bind(m_sock_fd, m_addr.get_sockaddr(), m_addr.size());
    return_value_check_equal(ret, 0);

    ret = ::listen(m_sock_fd, SOMAXCONN);
    return_value_check_equal(ret, 0);
}