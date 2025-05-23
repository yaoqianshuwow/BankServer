#pragma once
#include "Epoll.h"
#include<queue>
#include<deque>
#include<functional>
#include<mutex>
#include <sys/eventfd.h>
#include"Channel.h"
class Channel;
class Epoll;

// 事件循环类。
class EventLoop
{
private:
    Epoll *ep_;                       // 每个事件循环只有一个Epoll。
    pid_t threadid_;
    std::vector<function<void()>>loopque_;
    int wakefd_;
    
    Channel*wakechannel_;
    mutex mutex_;
public:
    EventLoop();                   // 在构造函数中创建Epoll对象ep_。
    ~EventLoop();                // 在析构函数中销毁ep_。

    void run();                      // 运行事件循环。

    void updatechannel(Channel *ch);  // 把channel添加/更新到红黑树上，channel中有fd，也有需要监视的事件。
    
    Epoll*ep();
    bool isinloopthread();
    void queueinloopthread(function<void()>);


    void wakeup();
    void handlewakeup();
};