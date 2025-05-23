#pragma once
#include <sys/epoll.h>
#include "Epoll.h"
#include"Socket.h"
#include"InetAddress.h"
#include"EventLoop.h"
#include<functional>
using namespace std;
class Epoll;

class EventLoop;

class Channel
{
private:
    int fd_=-1;                             // Channel拥有的fd，Channel和fd是一对一的关系。
    //Epoll *ep_=nullptr;                // Channel对应的红黑树，Channel与Epoll是多对一的关系，一个Channel只对应一个Epoll。
    bool inepoll_=false;              // Channel是否已添加到epoll树上，如果未添加，调用epoll_ctl()的时候用EPOLL_CTL_ADD，否则用EPOLL_CTL_MOD。
    uint32_t events_=0;              // fd_需要监视的事件。listenfd和clientfd需要监视EPOLLIN，clientfd还可能需要监视EPOLLOUT。
    uint32_t revents_=0;             // fd_已发生的事件。 
    bool islisten_=false;
    std::function<void()>readcallback_;         // fd_读事件的回调函数。
    std::function<void()>writecallback_;
    std::function<void()>closecallback_;
    std::function<void()>errorcallback_;
    EventLoop*loop_=nullptr;

public:
    Channel(EventLoop* loop,int fd);      // 构造函数。
    ~Channel();                           // 析构函数。 

    int fd();                                            // 返回fd_成员。
    void useet();                                    // 采用边缘触发。
    void enablereading();                     // 让epoll_wait()监视fd_的读事件。
    void disablereading();                    // 取消读事件。
    void enablewriting();                      // 注册写事件。
    void disablewriting();                     // 取消写事件。

    void setinepoll();;                           // 把inepoll_成员的值设置为true。
    void setrevents(uint32_t ev);         // 设置revents_成员的值为参数ev。
    bool inpoll();                                  // 返回inepoll_成员。
    uint32_t events();                           // 返回events_成员。
    uint32_t revents();                          // 返回revents_成员。
    void handleevent();


    
    //void onmessage();                                     // 处理对端发送过来的消息。
    void setreadcallback(std::function<void()> fn);    // 设置fd_读事件的回调函数。
    void setwritecallback(std::function<void()> fn);
    void setclosecallback(std::function<void()>fn);
    void seterrorcallback(std::function<void()>fn);
};