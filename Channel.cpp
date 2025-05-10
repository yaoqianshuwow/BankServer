#include"Channel.h"


Channel::Channel(EventLoop* loop,int fd):loop_(loop),fd_(fd)   // 构造函数。
{

}

Channel::~Channel()                           // 析构函数。 
{
    // 在析构函数中，不要销毁ep_，也不能关闭fd_，因为这两个东西不属于Channel类，Channel类只是需要它们，使用它们而已。
}

int Channel::fd()                                            // 返回fd_成员。
{
    return fd_;
}

void Channel::useet()                                    // 采用边缘触发。
{
    events_=events_|EPOLLET;
}
void Channel::enablereading(){
   
    events_|=EPOLLIN;
    loop_->ep()->updatechannel(this);
}
void Channel::enablewriting(){

    events_|=EPOLLOUT;
    loop_->ep()->updatechannel(this);
}
void Channel::disablereading(){
    events_&=~EPOLLIN;
    loop_->ep()->updatechannel(this);
}
void Channel::disablewriting(){
    events_&=~EPOLLOUT;
    loop_->ep()->updatechannel(this);
}
void Channel::setinepoll()                           // 把inepoll_成员的值设置为true。
{
    inepoll_=true;
}

void Channel::setrevents(uint32_t ev)         // 设置revents_成员的值为参数ev。
{
    revents_=ev;
}
bool Channel::inpoll(){  
    
    return inepoll_;
}

uint32_t Channel::events()                           // 返回events_成员。
{
    return events_;
}

uint32_t Channel::revents()                          // 返回revents_成员。
{
    return revents_;
}
void Channel::handleevent(){


    if (revents_ & EPOLLRDHUP) {                     // 对方已关闭，有些系统检测不到，可以使用 EPOLLIN，recv() 返回 0。
        
        closecallback_;
    }  
    else if (revents_ & (EPOLLIN | EPOLLPRI)) {   // 接收缓冲区中有数据可以读。
        
        readcallback_();
        
    }
    else if (revents_ & EPOLLOUT)                  // 有数据需要写，暂时没有代码，以后再说。
    {
        writecallback_();
    }
    else                                                                   // 其它事件，都视为错误。
    {
        printf("huidiaohanshu3\n");
        errorcallback_();
    }





}




void Channel::setreadcallback(std::function<void()> fn) {



    readcallback_=fn;
}   

void Channel::setwritecallback(std::function<void()> fn){

    writecallback_=fn;
}
void Channel::setclosecallback(std::function<void()>fn){

  closecallback_=fn;

}
void Channel::seterrorcallback(std::function<void()>fn){


    errorcallback_=fn;
}

