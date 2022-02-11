#ifndef NETADDR_H
#define NETADDR_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include "utils.hpp"


class inet_utils {


};


class inet_address {
public:

  inet_address(bool is_ipv4):inet_address(0, true, is_ipv4){

  }

  inet_address(struct sockaddr_in in_addr) : m_in_addr(in_addr) {

  }

  inet_address(struct sockaddr_in6 in_addr6) : m_in_addr6(in_addr6) {

  }

  inet_address(uint16_t port, bool is_any, bool is_ipv4);

  

  const struct sockaddr_in& get_addr_in () const {
      return m_in_addr;
  }

  const struct sockaddr_in6& get_addr_in6() const {
      return m_in_addr6;
  }

  struct sockaddr* get_sockaddr() {
      return (struct sockaddr*) (&m_in_addr);
  }

  bool is_ipv4 () {
      return get_sockaddr()->sa_family == AF_INET;
  }

  socklen_t size() {
      return is_ipv4() ? sizeof (sockaddr_in) : sizeof (sockaddr_in6);
  }


private:
    union {
        struct sockaddr_in m_in_addr;
        struct sockaddr_in6 m_in_addr6;
    };
};



#endif