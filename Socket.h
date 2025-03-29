#pragma once
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/tcp.h>


#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "InetAddress.h"
using namespace std;
// 创建一个非阻塞的socket。
int createnonblocking();

// socket类。
class Socket
{
private:
    const int fd_;                // Socket持有的fd，在构造函数中传进来。
    string ip_;
    uint16_t port_;
public:
    Socket(int fd);             // 构造函数，传入一个已准备好的fd。
    ~Socket();                   // 在析构函数中，将关闭fd_。

    int fd() const;                              // 返回fd_成员。
    void setreuseaddr(bool on);       // 设置SO_REUSEADDR选项，true-打开，false-关闭。
    void setreuseport(bool on);       // 设置SO_REUSEPORT选项。
    void settcpnodelay(bool on);     // 设置TCP_NODELAY选项。
    void setkeepalive(bool on);       // 设置SO_KEEPALIVE选项。
    void bind(const InetAddress& servaddr);        // 服务端的socket将调用此函数。
    void listen(int nn=128);                                    // 服务端的socket将调用此函数。
    int   accept(InetAddress& clientaddr);            // 服务端的socket将调用此函数。

    string ip()const;
    uint16_t port()const;
};