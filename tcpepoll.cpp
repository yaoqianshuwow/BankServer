#include<iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>          
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>      // TCP_NODELAY需要包含这个头文件

#include"InetAddress.h"
// #include"Socket.h"
// #include"Epoll.h"
// #include"Channel.h"
// #include"EventLoop.h"
#include"TcpServer.h"
#include"EchoServer.h"
using namespace std;
// 设置非阻塞的IO
// void setnonblocking(int fd)
// {
//     fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK).
// }

int main(int argc,char *argv[])
{  
    if (argc != 3) 
    { 
        printf("usage: ./tcpepoll ip port\n"); 
        printf("example: ./tcpepoll 192.168.201.128 5085\n\n");
        return -1;
    }
   
  
 

 
  EchoServer echoserver(argv[1],atoi(argv[2]),3,2);
  echoserver.Start();
  return 0;
}