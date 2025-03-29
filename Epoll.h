#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>


#include <sys/epoll.h>

#include <vector>
#include <unistd.h>
using namespace std;

#include"Channel.h"


class Channel;


// Epoll类。
class Epoll
{
private:
    static const int MaxEvents=100;                   // epoll_wait()返回事件数组的大小。
    int epollfd_=-1;                                             // epoll句柄，在构造函数中创建。
    epoll_event events_[MaxEvents];                  // 存放poll_wait()返回事件的数组，在构造函数中分配内存。
public:
    Epoll();                                             // 在构造函数中创建了epollfd_。
    ~Epoll();                                          // 在析构函数中关闭epollfd_。

    //void addfd(int fd, uint32_t op);                             // 把fd和它需要监视的事件添加到红黑树上。

    void updatechannel(Channel*ch);
    
    vector<Channel*> loop(int timeout=-1);   // 运行epoll_wait()，等待事件的发生，已发生的事件用vector容器返回。
};