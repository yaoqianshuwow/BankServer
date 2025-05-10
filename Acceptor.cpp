#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *loop,const std::string &ip,const uint16_t port):loop_(loop),servsock_(createnonblocking()),acceptchannel_(loop_,servsock_.fd())
{
      
    InetAddress servaddr(ip,port);             // 服务端的地址和协议。
    servsock_.setreuseaddr(true);
    servsock_.settcpnodelay(true);
    servsock_.setreuseport(true);
    servsock_.setkeepalive(true);
    servsock_.bind(servaddr);
    servsock_.listen();

          
    //acceptchannel_->setreadcallback(bind(&TcpServer::newconnection));
    acceptchannel_.setreadcallback(bind(&Acceptor::newconnection,this));
   
    acceptchannel_.enablereading();       // 让epoll_wait()监视servchannel的读事件。 
}

Acceptor::~Acceptor()
{
    
}

void Acceptor::setnewconnectioncb(std::function<void(Socket*)> fn){


newconnectioncb_=fn;
} 
void Acceptor::newconnection(){

    sockaddr_in peeraddr;

    InetAddress clientaddr(peeraddr);             // 客户端的地址和协议。
    // 注意，clientsock只能new出来，不能在栈上，否则析构函数会关闭fd。
    // 还有，这里new出来的对象没有释放，这个问题以后再解决。
    Socket*clientsock=new Socket(servsock_.accept(clientaddr));
    clientsock->setipport(clientaddr.ip(),clientaddr.port());

    //printf ("accept client(fd=%d,ip=%s,port=%d) ok.\n",clientsock->fd(),clientaddr.ip(),clientaddr.port());
    
    newconnectioncb_(clientsock);

}
