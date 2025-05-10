#include"Epoll.h"
// class Epoll
// {
// private:
//     static const int MaxEvents=100;                   // epoll_wait()返回事件数组的大小。
//     int epollfd_=-1;                                             // epoll句柄，在构造函数中创建。
//     epoll_event events_[MaxEvents];                  // 存放poll_wait()返回事件的数组，在构造函数中分配内存。
// public:
//     Epoll();                                             // 在构造函数中创建了epollfd_。
//     ~Epoll();                                          // 在析构函数中关闭epollfd_。

//     void addfd(int fd, uint32_t op);                             // 把fd和它需要监视的事件添加到红黑树上。
//     std::vector<epoll_event> loop(int timeout=-1);   // 运行epoll_wait()，等待事件的发生，已发生的事件用vector容器返回。
// };
Epoll::Epoll(){
    if ((epollfd_=epoll_create(1))==-1)       // 创建epoll句柄（红黑树）。
    {
        printf("epoll_create() failed(%d).\n",errno); exit(-1);
    }
    

}
Epoll::~Epoll(){
    close(epollfd_);
}
// void Epoll::addfd(int fd,uint32_t op){
//   epoll_event ev;
//   ev.data.fd=fd;
//   ev.events=op;
//   if (epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&ev)==-1)     // 把需要监视的fd和它的事件加入epollfd中。
//   {
//       printf("epoll_ctl() failed(%d).\n",errno); exit(-1);
//   }
// }
void Epoll::updatechannel(Channel*ch){
    epoll_event ev;
    ev.data.ptr=ch;
    ev.events=ch->events();
    if (ch->inpoll())         // 如果channel已经在树上了。
    {
    if (epoll_ctl(epollfd_,EPOLL_CTL_MOD,ch->fd(),&ev)==-1)     // 把需要监视的fd和它的事件加入epollfd中。
    {
        printf("epoll_ctl()mod failed(%d).\n",errno); exit(-1);
    }


    }
    else{

  if (epoll_ctl(epollfd_,EPOLL_CTL_ADD,ch->fd(),&ev)==-1)     // 把需要监视的fd和它的事件加入epollfd中。
    {
        printf("epoll_ctl()add failed(%d).\n",errno); exit(-1);
    }
    ch->setinepoll();


    }

}


// 运行epoll_wait()，等待事件的发生，已发生的事件用vector容器返回。
std::vector<Channel *> Epoll::loop(int timeout)   
{
    std::vector<Channel *> channels;        // 存放epoll_wait()返回的事件。

    bzero(events_,sizeof(events_));
    int infds=epoll_wait(epollfd_,events_,MaxEvents,timeout);       // 等待监视的fd有事件发生。

    // 返回失败。
    if (infds < 0)
    {
        perror("epoll_wait() failed"); exit(-1);
    }

    // 超时。
    if (infds == 0)
    {
        printf("epoll_wait() timeout.\n"); return channels;
    }
    //printf("jisdbcijsab\n");
    // 如果infds>0，表示有事件发生的fd的数量。
    for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组events_。
    {
        // evs.push_back(events_[ii]);
        Channel *ch=(Channel *)events_[ii].data.ptr;       // 取出已发生事件的channel。
        ch->setrevents(events_[ii].events);                       // 设置channel的revents_成员。
        channels.push_back(ch);
    }

    return channels;
}