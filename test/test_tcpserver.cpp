#include "eventloop.hpp"
#include "tcpserver.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>




void message_cb(const tcp_server::tcp_connector_sharep& p, small_buffer& buf) {

}

int main() {
    std::unique_ptr<eventloop> loop(new eventloop);
    inet_address listen_addr(5015, true, true);
    tcp_server server(loop.get(), tcp_server::message_func(&message_cb), listen_addr);
    loop->loop();
}
