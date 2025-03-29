#pragma once
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"

class Connection
{
private:
    EventLoop *loop_;               // Connection对应的事件循环，在构造函数中传入。 
    Socket *clientsock_;             // 与客户端通讯的Socket。
    Channel *clientchannel_;     // Connection对应的channel，在构造函数中创建。
    
public:
    Connection(EventLoop *loop,Socket *clientsock);
    ~Connection();
    string ip()const;
    uint16_t port()const;
    int fd()const;
    
};
