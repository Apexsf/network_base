#include "iohandler.hpp"
#include "eventloop.hpp"
#include "utils.hpp"


iohandler::iohandler(int fd, eventloop* loop):m_fd(fd), m_loop(loop),
m_event(EPOLLIN), m_op(poller_op::NONE) {
    set_fd_nonblocking();
}

iohandler::poller_op iohandler::get_op() {
    return m_op;
}

void iohandler::set_fd_nonblocking() {
    ::set_fd_nonblocking(m_fd);
}

int iohandler::get_event() {
    return m_event;
}

int iohandler::get_fd() {
    return m_fd;
}

void iohandler::add_to_poller() {
    m_op = poller_op::ADD;
    m_loop->update(this);
}

void iohandler::mod_on_poller() {
    m_op = poller_op::MOD;
    m_loop->update(this);
}

void iohandler::delete_from_poller() {
    m_op = poller_op::DELETE;
    m_loop->update(this);
}

void iohandler::set_ev_read() {
    m_event = EPOLLIN;
}

void iohandler::set_ev_write() {
    m_event = EPOLLOUT;
}

void iohandler::set_polled_event(int event) {
    m_polled_event = event;
}

void iohandler::set_read_cb(read_func cb) {
    m_read_cb = std::move(cb);
}

void iohandler::set_write_cb(write_func cb) {
    m_write_cb = std::move(cb);
}




void iohandler::handle_event(){
    if (m_polled_event & (EPOLLIN | EPOLLPRI | EPOLLHUP)) {
        m_read_cb();
    }
}