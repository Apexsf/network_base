#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include "utils.hpp"
#include "inetaddr.hpp"

struct socket_util {
    
};




class general_socket {
public:
    /**
     * @brief Construct a new socket object from an existed socketfd
     * 
     * @param fd 
     */
    general_socket(int fd);

    /**
     * @brief Construct a new socket object by calling system calls
     * 
     * @param domain 
     * @param type 
     * @param protocol 
     */
    general_socket(int domain, int type, int protocol);

    int get_sock_fd() {
        return m_sock_fd;
    }

protected:
    int m_sock_fd;
};


class listening_socket : public general_socket {
public:
    /**
     * @brief construct listen_socket from an initialized address
     * 
     * @param in_addr 
     */
    listening_socket(inet_address in_addr);
    /**
     * @brief construct listen_socket from a given port
     * 
     * @param port given port
     * @param is_any if true, set bind address too INADDR_ANY, else INADDR_LOOPBACK
     */
    listening_socket(uint16_t port, bool is_any = true, bool is_ipv4 = true);
    

private:
    /**
     * @brief applying bind and listen system call based on the current sockfd and inet address
     * 
     */
    void init();
    inet_address m_addr;
};








#endif