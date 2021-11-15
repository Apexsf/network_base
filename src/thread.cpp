#include "thread.hpp"


void* thread::run(void* args){
    thread* th = static_cast<thread*>(args);
    th->m_running = true;
    th->m_task();

    return nullptr;
}


thread::thread(TASK task) : m_task(task), m_running(false) {
    
}   

thread::~thread() {
    if (m_running){
        join();
    }

}

bool thread::is_running(){
    return m_running;
}

void thread::join(){
    return_value_check_equal( pthread_join(m_pthread_t,NULL), 0);
    this->m_running = false;

}

void thread::start() {
    return_value_check_equal( pthread_create(&m_pthread_t, NULL, run, this), 0);
}

pthread_t thread::get_pthread_t() {
    return m_pthread_t;
}



//m_running is not defined
// void thread::detach(){
//     return_value_check_equal( pthread_detach(m_pthread_t), 0);
// }






