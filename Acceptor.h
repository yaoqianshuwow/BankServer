#pragma once
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"
class TcpServer;
class Acceptor
{
private:
    EventLoop *loop_;               // Acceptor对应的事件循环，在构造函数中传入。 
    Socket *servsock_;               // 服务端用于监听的socket，在构造函数中创建。
    Channel *acceptchannel_;   // Acceptor对应的channel，在构造函数中创建。
    std::function<void(Socket*)> newconnectioncb_;
public:
    Acceptor(EventLoop *loop,const std::string &ip,const uint16_t port);
    ~Acceptor();
   
    void setnewconnectioncb(std::function<void(Socket*)> fn);    
    void newconnection();
};
