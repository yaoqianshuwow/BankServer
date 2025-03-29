// class Connection
// {
// private:
//     EventLoop *loop_;               // Connection对应的事件循环，在构造函数中传入。 
//     Socket *clientsock_;             // 与客户端通讯的Socket。
//     Channel *clientchannel_;     // Connection对应的channel，在构造函数中创建。
// public:
//     Connection(EventLoop *loop,Socket *clientsock);
//     ~Connection();
// };
#include"Connection.h"
Connection::Connection(EventLoop *loop,Socket *clientsock):loop_(loop),clientsock_(clientsock)
{
    clientchannel_=new Channel(loop_,clientsock_->fd());   // 这里new出来的对象没有释放，这个问题以后再解决。
    clientchannel_->useet();                 // 客户端连上来的fd采用边缘触发。
    clientchannel_->enablereading();   // 让epoll_wait()监视clientchannel的读事件
    clientchannel_->setreadcallback(bind(&Channel::onmessage,clientchannel_));
}
Connection::~Connection(){

delete clientchannel_;
delete clientsock_;

}
string Connection::ip()const{

return clientsock_->ip();
}
uint16_t Connection::port()const{
return clientsock_->port();
}
int Connection::fd()const{
    return clientsock_->fd();
}