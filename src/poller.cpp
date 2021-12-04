#include "poller.hpp"


epoller::epoller() :  m_event_buf(EPOLL_MIN_EVENT) {
    m_epoll_fd = epoll_create(1);
    return_value_check_more(m_epoll_fd, -1);
}

void epoller::update(iohandler* handler) {
    using poller_op = iohandler::poller_op;
    poller_op op = handler->get_op();
    
    switch (op) {
        case poller_op::ADD:{
            add(handler);
            break;
        }
        case poller_op::MOD:{
            mod(handler);
            break;
        }
        case poller_op::DELETE:{
            del(handler);
            break;
        }
        default:
            LOG_ERROR << "poller_op being updated can not be set to NONE";
            break;
    }
}

void epoller::add(iohandler* handler) {
    int ret;
    struct epoll_event event;
    event.events = handler->get_event();
    event.data.ptr = (void*) handler;
    ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, handler->get_fd(),&event);
    return_value_check_equal(ret, 0);
}


void epoller::mod(iohandler* handler) {
    int ret;
    struct epoll_event event;
    event.events = handler->get_event();
    event.data.ptr = (void*) handler;
    ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, handler->get_fd(),&event);
    return_value_check_equal(ret, 0);

}

void epoller::del(iohandler* handler) {
    int ret;
    // for EPOLL_CTL_DEL, The event is ignored and can be NULL, but bugs will be encountered when linux kernel version is before 2.6.9 (see `man epoll_ctl`). Thus for portability, an uninitialized event will be defined and pass to epoll_ctl
    struct epoll_event event;
    ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, handler->get_fd(), &event);
    return_value_check_equal(ret, 0);
}

void epoller::poll(std::vector<iohandler*>& handlers, int timeout) {
    int num = epoll_wait(m_epoll_fd, m_event_buf.data(), m_event_buf.size(), timeout);
    if (num == -1) {
        LOG_ERROR << "EPOLL WAIT ERROR," << "errno : " << errno << ", " << get_errno_name(errno);
        return;
    }
    for(int i = 0; i < num; i++) {
        struct epoll_event& event = m_event_buf[i];
        iohandler* handler = (iohandler*)event.data.ptr;
        handler->set_polled_event(event.events);
        handlers.push_back(handler);
    }

    if (num == m_event_buf.size() && num <= EPOLL_MAX_EVENT) {
        int new_size = m_event_buf.size();
        m_event_buf.resize(new_size*2);
    }
    
}


