#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "iohandler.hpp"
#include "poller.hpp"


class eventloop{
public:
    eventloop();
    void update(iohandler*);

private:
    poller* m_poller;
};



#endif