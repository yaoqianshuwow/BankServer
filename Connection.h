#pragma once
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"
#include"Buffer.h"
#include<memory>
#include"Timestamp.h"
class Connection;
class EventLoop;
class Channel;
using spConnection=std::shared_ptr<Connection>;
class Connection:public std::enable_shared_from_this<Connection>
{
private:
    EventLoop *loop_;               // Connection对应的事件循环，在构造函数中传入。 
    Socket *clientsock_;             // 与客户端通讯的Socket。
    std::unique_ptr<Channel>clientchannel_;     // Connection对应的channel，在构造函数中创建。
    std::function<void(spConnection)>closecallback_;
    std::function<void(spConnection)>errorcallback_;
    std::function<void(spConnection,string &)>onmessagecallback_;

    Buffer inputbuffer_;
    Buffer outputbuffer_;
    //shared_ptr<Connection>shared_from_this;
    Timestamp ts;
public:
    Connection(EventLoop*loop,Socket *clientsock);
    ~Connection();
    string ip()const;
    uint16_t port()const;
    int fd()const;
    void closecallback();
    void errorcallback();
    void writecallback(); 
    void setclosecallback(std::function<void(spConnection)>fn);
    void seterrorcallback(std::function<void(spConnection)>fn);
    void setonmessagecallback(std::function<void(spConnection,string )>fn);
    void onmessage();
    void send(const char *data,size_t size);
    void sendinloop(const char *data,size_t size);
    int laststamp();
};
