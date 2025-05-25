#pragma once
#include "Epoll.h"
#include<queue>
#include<deque>
#include<functional>
#include<mutex>
#include <sys/eventfd.h>
#include<time.h>
#include"Channel.h"
#include <sys/timerfd.h>
#include<set>
#include<unordered_map>

#include<map>
#include<memory>
#include"Connection.h"


#include"Timestamp.h"
class Channel;
class Epoll;
class Connection;
using spConnection=std::shared_ptr<Connection>;
struct node{

int t;
int fd;  

};
// 事件循环类。
class EventLoop
{
private:
    Epoll *ep_;                       // 每个事件循环只有一个Epoll。
    pid_t threadid_;


    std::vector<function<void()>>loopque_;
    int wakefd_;
    Channel*wakechannel_;//需不需要删除？？？？
    mutex mutex_;
  
    bool mainloop_;
    int timefd;
    Channel*timechannel_;
    
    map<int,spConnection>conns;

    function<void(spConnection)>timeoutcb_;
   
   
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
    
    void settimeout(bool mainloop,int sec);
    void handletimeout(int sec);
    void newconnewcton(int fd,spConnection conn);

    void settimeout(function<void(spConnection)>fn);
};