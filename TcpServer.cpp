#include"TcpServer.h"
// class TcpServer
// {
// private:
//     EventLoop loop_;         // 一个TcpServer可以有多个事件循环，现在是单线程，暂时只用一个事件循环。
// public:
//     TcpServer(const std::string &ip,const uint16_t port);
//     ~TcpServer();

//     void start();          // 运行事件循环。
// };
TcpServer::TcpServer(const std::string &ip,const uint16_t port){

acceptor_=new Acceptor(&loop_,ip,port);
acceptor_->setnewconnectioncb(bind(&TcpServer::newconnection,this,placeholders::_1));    
}
TcpServer::~TcpServer(){
for(auto &it : conns)delete it.second;
}
void TcpServer::start(){

 loop_.run();

}

void TcpServer::newconnection(Socket *clientsock)
{  
    
   Connection *conn=new Connection(&loop_,clientsock);   // 这里new出来的对象没有释放，这个问题以后再解决。
   printf ("newconnection(fd=%d,ip=%s,port=%d) ok.\n",clientsock->fd(),conn->ip(),conn->port());
   conns[conn->fd()]=conn;
}
