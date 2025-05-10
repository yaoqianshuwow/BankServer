#include"Socket.h"


// // 创建一个非阻塞的socket。
// int createnonblocking();

// // socket类。
// class Socket
// {
// private:
//     const int fd_;                // Socket持有的fd，在构造函数中传进来。
// public:
//     Socket(int fd);             // 构造函数，传入一个已准备好的fd。
//     ~Socket();                   // 在析构函数中，将关闭fd_。

//     int fd() const;                              // 返回fd_成员。
//     void setreuseaddr(bool on);       // 设置SO_REUSEADDR选项，true-打开，false-关闭。
//     void setreuseport(bool on);       // 设置SO_REUSEPORT选项。
//     void settcpnodelay(bool on);     // 设置TCP_NODELAY选项。
//     void setkeepalive(bool on);       // 设置SO_KEEPALIVE选项。
//     void bind(const InetAddress& servaddr);        // 服务端的socket将调用此函数。
//     void listen(int nn=128);                                    // 服务端的socket将调用此函数。
//     int   accept(InetAddress& clientaddr);            // 服务端的socket将调用此函数。
// };
// int opt = 1; 
//     setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。
//     setsockopt(listenfd,SOL_SOCKET,TCP_NODELAY   ,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。
//     setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT ,&opt,static_cast<socklen_t>(sizeof opt));    // 有用，但是，在Reactor中意义不大。
//     setsockopt(listenfd,SOL_SOCKET,SO_KEEPALIVE   ,&opt,static_cast<socklen_t>(sizeof opt));    // 可能有用，但是，建议自己做心跳。

int createnonblocking(){
 // 创建服务端用于监听的listenfd。
 int listenfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
 if (listenfd < 0)
 {
     // perror("socket() failed"); exit(-1);
     printf("%s:%s:%d listen socket create error:%d\n", __FILE__, __FUNCTION__, __LINE__, errno); exit(-1);
 }
 return listenfd;

    //fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
Socket::Socket(int fd):fd_(fd){

}
// 在析构函数中，将关闭fd_。
Socket::~Socket()
{
    ::close(fd_);
}

int Socket::fd() const                              // 返回fd_成员。
{
    return fd_;
}
void Socket::settcpnodelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)); // TCP_NODELAY包含头文件 <netinet/tcp.h>
}

void Socket::setreuseaddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); 
}

void Socket::setreuseport(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); 
}

void Socket::setkeepalive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)); 
}

void Socket::bind(const InetAddress&servaddr){
   ::bind(fd_,servaddr.addr(),sizeof(sockaddr));
   setipport(servaddr.ip(),servaddr.port());
}
void Socket::listen(int nn){
    ::listen(fd_,nn) != 0 ;
}
int  Socket::accept(InetAddress& clientaddr){
    socklen_t len=sizeof(clientaddr);
   return accept4(fd_,(sockaddr*)&clientaddr,&len,SOCK_NONBLOCK);


}

string Socket:: ip()const{

    return ip_;
}
uint16_t Socket::port()const{

    return port_;
}
void Socket::setipport(const string&ip,const uint16_t &port){


    ip_=ip;
    port_=port;
}