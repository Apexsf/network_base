#ifndef POLLER_H
#define POLLER_H

#include <vector>   
#include <sys/epoll.h>
#include "iohandler.hpp"
#include "logger.hpp"

#define EPOLL_MAX_EVENT 1024
#define EPOLL_MIN_EVENT 16

class poller {
public:
    poller() = default;
    virtual void poll (std::vector<iohandler*>&, int timeout ) = 0;
};


/**
 * @brief epoll-based poller 
 * 
 */
class epoller  {
public:
    static epoller* get_epoller() {
        static epoller epoller_instance;
        return &epoller_instance;
    }

    void poll (std::vector<iohandler*>&, int timeout);

    /**
     * @brief update handler, either by add, mod or del.
     * The operations in terms of epoll fully depend on 
     * the passed handler
     * 
     */
    void update (iohandler* handler);
    
private:
    epoller();
    

    void add(iohandler*);
    void mod(iohandler*);
    void del(iohandler*);
    
    std::vector<struct epoll_event> m_event_buf;
    int m_epoll_fd;
};



#endif