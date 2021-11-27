#include "eventloop.hpp"




eventloop::eventloop():m_poller(epoller::get_epoller()),
    m_timer_seq(std::make_unique<timer_sequence>())
 {

}

void eventloop::update(iohandler* handler) {
    m_poller->update(handler);
}


void eventloop::loop() {
    std::vector<iohandler*> polling_handlers;
    while(true) {
        polling_handlers.clear();
        m_poller->poll(polling_handlers,2000);
        for (iohandler* handler : polling_handlers) {
            handler->handle_event();
        }
    }

}

