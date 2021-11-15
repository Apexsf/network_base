#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <queue>
#include <vector>
#include <future>
#include <memory>
#include "mutex.hpp"
#include "condition.hpp"
#include "thread.hpp"


class threadpool {
public:
    using func_type = std::function<void()>;

    threadpool(int size = 8);
    ~threadpool();
    threadpool(threadpool&) = delete;
    threadpool& operator=(threadpool&) = delete;
    
    void stop();

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;


private:    
    std::vector<std::unique_ptr<thread>> m_ths;
    std::queue<func_type> m_que;
    int m_size;
    bool m_running;
    mutex m_mut;
    condition m_cond;
};

template<typename F, typename... Args>
auto threadpool::submit(F&& f, Args&& ...args)->std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> res = task->get_future();
    lock_guard lk(m_mut);
    m_que.emplace([task](){(*task)();});
    m_cond.notify();
    return res;
}










#endif