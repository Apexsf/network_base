#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <functional>
#include <sys/epoll.h>

class eventloop;

class iohandler {
public:
    using read_func = std::function<void()>;
    using write_func = std::function<void()>;

    /**
     * @brief Construct a new iohandler object, note that any fd passed
     * to this ctr will be set to non-blocking.
     * 
     * @param fd 
     * @param loop 
     */
    iohandler(int fd, eventloop* loop);
    enum class poller_op {
        ADD = 0,
        MOD,
        DELETE,
        NONE
    };

    void set_fd_nonblocking();

    void add_to_poller();
    void mod_on_poller();
    void delete_from_poller();

    void set_ev_read();
    void set_ev_write();

    void set_read_cb(read_func);
    void set_write_cb(write_func);
    void set_polled_event(int);

    int get_event();
    poller_op get_op();
    int get_fd();

    void handle_event();


private:
    read_func m_read_cb;
    write_func m_write_cb;
    poller_op m_op;
    int m_event;
    int m_fd;
    int m_polled_event;
    eventloop* m_loop;
};







#endif