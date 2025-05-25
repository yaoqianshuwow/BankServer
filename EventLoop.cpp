#include"EventLoop.h"
// // 事件循环类。
// class EventLoop
// {
// private:
//     Epoll *ep_;                       // 每个事件循环只有一个Epoll。
// public:
//     EventLoop();                   // 在构造函数中创建Epoll对象ep_。
//     ~EventLoop();                // 在析构函数中销毁ep_。

//     void run();                      // 运行事件循环。

//     void updatechannel(Channel *ch);                        // 把channel添加/更新到红黑树上，channel中有fd，也有需要监视的事件。
// };
EventLoop::EventLoop():ep_(new Epoll),wakefd_(eventfd(0,EFD_NONBLOCK)),wakechannel_(new Channel(this,wakefd_)),mainloop_(false){

//wakechannel_->useet();                 // 客户端连上来的fd采用边缘触发。
    

wakechannel_->setreadcallback(bind(&EventLoop::handlewakeup,this));
wakechannel_->enablereading();   // 让epoll_wait()监视clientchannel的读事件
//wakeup();
}
EventLoop::~EventLoop(){

    delete ep_;
}
void EventLoop::updatechannel(Channel*ch){
ep_->updatechannel(ch);
}
#include <sys/syscall.h>
using namespace std;
#include<iostream>
void EventLoop::run(){
    threadid_=syscall(SYS_gettid);
    std::cout<<"loop函数的线程id:"<<threadid_<< std::endl;
    
    while (true)
    {
     
      vector<Channel *> channels=ep_->loop();         // 等待监视的fd有事件发生。可以填入超时时间
  
      for (auto &ch:channels)
      {
  
          ch->handleevent();        
                
      }
  
    }
}
bool EventLoop::isinloopthread(){

    
    return (threadid_==static_cast<pid_t>(syscall(SYS_gettid)));
}
void EventLoop::queueinloopthread(function<void()>fn){
  {
        std::lock_guard<std::mutex> gd(mutex_);           // 给任务队列加锁。
        loopque_.push_back(fn);                                            // 任务入队。
    }

    wakeup();        // 唤醒事件循环。
   
}
Epoll* EventLoop::ep(){
    return ep_;
}
void EventLoop::wakeup(){
    //不要重复加锁
    uint64_t val=1;//无符号64位int
    write(wakefd_,&val,sizeof(val));
    
}

void EventLoop::handlewakeup(){
    printf("handlewakeup() thread id is %d.\n",syscall(SYS_gettid));

    uint64_t val;
    read(wakefd_,&val,sizeof(val));       // 从eventfd中读取出数据，如果不读取，eventfd的读事件会一直触发。

    std::function<void()> fn;

    std::lock_guard<std::mutex> gd(mutex_);           // 给任务队列加锁。

    
   
    for(auto it=loopque_.begin();it!=loopque_.end();){
        fn=*it;
        fn();
       
        loopque_.erase(it);
        
        std::cout << "Task executed, remaining queue size: " << loopque_.size() << std::endl;
    
    }
    
}


void EventLoop::settimeout(bool mainloop,int sec){

    mainloop_=mainloop;
    
    if(mainloop_)return;
    //else cout<<"subloop定时器"<<endl;
    timefd=timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC|TFD_NONBLOCK);
    struct itimerspec timeout;

    memset(&timeout,0,sizeof(itimerspec));

    timeout.it_value.tv_sec=5;
    timeout.it_value.tv_nsec=0;

    timeout.it_interval.tv_sec = 5;
    timerfd_settime(timefd,0,&timeout,0);
    timechannel_=new Channel(this,timefd);
    timechannel_->setreadcallback(bind(&EventLoop::handletimeout,this,sec));
    timechannel_->enablereading();
}
void EventLoop::handletimeout(int sec){
    uint64_t expirations;
   read(timefd, &expirations, sizeof(expirations));
    if(!mainloop_){

   for(auto  it:conns){

    cout<<it.first<<" ";
    if(Timestamp::now().toint()-it.second->laststamp()>=sec){
        conns.erase(it.first);
        timeoutcb_(it.second);
    }
   }
   cout<<endl;


    }
    

}
void EventLoop::newconnewcton(int fd,spConnection conn){


  conns[fd]=conn;
 }
void EventLoop::settimeout(function<void(spConnection)>fn){

    timeoutcb_=fn;
}