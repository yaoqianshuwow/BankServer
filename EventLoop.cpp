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
EventLoop::EventLoop():ep_(new Epoll){



}
EventLoop::~EventLoop(){

    delete ep_;
}
void EventLoop::updatechannel(Channel*ch){
ep_->updatechannel(ch);
}
void EventLoop::run(){


    while (true)
    {
     
      vector<Channel *> channels=ep_->loop();         // 等待监视的fd有事件发生。
  
      for (auto &ch:channels)
      {
  
          ch->handleevent();        
                
      }
  
    }
}
Epoll* EventLoop::ep(){
    return ep_;
}