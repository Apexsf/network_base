#include "eventloop.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

void setnonblocking(int sock) {
    int opt;

    opt = fcntl(sock, F_GETFL);
    if (opt < 0) {
        printf("fcntl(F_GETFL) fail.");
    }
    opt |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opt) < 0) {
        printf("fcntl(F_SETFL) fail.");
    }
}

int get_listen_fd() {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5003);

    int ret;
    ret = bind(listenfd, (struct sockaddr*)(&serv_addr), sizeof(serv_addr) );
    return_value_check_equal(ret, 0);

    ret = listen(listenfd, 10);
    return_value_check_equal(ret, 0);

    setnonblocking(listenfd);
    return listenfd;
}

class TCPConnecter{
public:
    void echo () {
        char buf[4096];
        int n  = ::recv(m_handler.get_fd(), (void*)(buf), 4096, MSG_DONTWAIT);
        if (n == 0) {// peer side closes connection
            m_handler.delete_from_poller();
            ::close(m_handler.get_fd());   
        }
        printf("recv %d words of message : %s", n, buf);
        ::send(m_handler.get_fd(), (void*) buf, n, 0);

    }
    
    TCPConnecter(int fd, eventloop* loop):m_handler(fd, loop) {
        m_handler.set_read_cb([this]() {this->echo();});
        m_handler.add_to_poller();
    }

private:
    iohandler m_handler;  // listenning fd
};

class TCPServer {
public:

    TCPServer(eventloop* loop) : m_loop(loop), m_handler(get_listen_fd(),loop) {
        
    }


    void accept (){
        int conn_fd = ::accept(m_handler.get_fd(), (struct sockaddr*)(nullptr), NULL);
        if (conn_fd < 0){
            // in fact, checking errno and perform the corresponding
            // operation is preferred
            return;
        }
        m_connectors.emplace_back(std::make_unique<TCPConnecter> (conn_fd, m_loop));
    }

    void start (){
        m_handler.set_read_cb([this](){{this->accept();}});
        m_handler.add_to_poller();
        m_loop->loop();
    }


private:
    iohandler m_handler; // listening handler
    std::vector<std::unique_ptr<TCPConnecter>> m_connectors;
    eventloop* m_loop;
};

int main() {
    std::unique_ptr<eventloop> loop(new eventloop);

    std::unique_ptr<TCPServer> server = std::make_unique<TCPServer>(loop.get());

    server->start();


}
