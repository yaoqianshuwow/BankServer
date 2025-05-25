#pragma once

#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

#include"Acceptor.h"
#include"Connection.h"
#include"ThreadPool.hpp"
#include<map>
#include<memory>

#include<mutex>
class Acceptor;

class TcpServer
{
private:
    EventLoop loop_;         // 一个TcpServer可以有多个事件循环，现在是单线程，暂时只用一个事件循环。
    Acceptor *acceptor_;
    ThreadPool pool_;
    mutex mt;
    map<int,spConnection>conns;
    int cnt=0;
    
    std::vector<unique_ptr<EventLoop>>subloop_;
    

    std::function<void(spConnection)> newconnectioncb_;          // 回调EchoServer::HandleNewConnection()。
    std::function<void(spConnection)> closeconnectioncb_;        // 回调EchoServer::HandleClose()。
    std::function<void(spConnection)> errorconnectioncb_;         // 回调EchoServer::HandleError()。
    std::function<void(spConnection,std::string &message)> onmessagecb_;        // 回调EchoServer::HandleMessage()。
    std::function<void(spConnection)> sendcompletecb_;            // 回调EchoServer::HandleSendComplete()。
    std::function<void(EventLoop*)>  timeoutcb_;                       // 回调EchoServer::HandleTimeOut()。
    
    void handeltimeout(spConnection conn);

public:
    TcpServer(const std::string &ip,const uint16_t port ,unsigned short threadnum);
    
    ~TcpServer();
    void newconnection(Socket*clientsock);
    void closeconnection(spConnection conn);
    void errorconnection(spConnection conn);
    void onmessage(spConnection conn,string  message);

    void start();          // 运行事件循环。



    void setnewconnectioncb(std::function<void(spConnection)> fn);
    void setcloseconnectioncb(std::function<void(spConnection)> fn);
    void seterrorconnectioncb(std::function<void(spConnection)> fn);
    void setonmessagecb(std::function<void(spConnection,std::string &message)> fn);
    void setsendcompletecb(std::function<void(spConnection)> fn);
    void settimeoutcb(std::function<void(EventLoop*)> fn);
};