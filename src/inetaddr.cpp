#include "inetaddr.hpp"

inet_address::inet_address(uint16_t port, bool is_any, bool is_ipv4) {
    if (is_ipv4) {
        mem_zero(&m_in_addr, sizeof (m_in_addr));
        m_in_addr.sin_family = AF_INET;
        m_in_addr.sin_port = htobe16(port);
        in_addr_t ip = is_any ? INADDR_ANY : INADDR_LOOPBACK;
        m_in_addr.sin_addr.s_addr = htobe32(ip);
    } else {
        mem_zero(&m_in_addr6, sizeof(m_in_addr6));
        m_in_addr6.sin6_family = AF_INET6;
        m_in_addr6.sin6_port = htobe16(port);
        in6_addr ip = is_any? in6addr_any : in6addr_loopback;
        m_in_addr6.sin6_addr = ip;
    }
}

