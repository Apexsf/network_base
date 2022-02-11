#include "connection.hpp"


connection::connection(int fd, eventloop* loop): m_handler(fd, loop){

    
};