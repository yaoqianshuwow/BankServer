# BankServer
网络银行，可以高并发接受请求报文，解决了分包问题，实现了定时清理空闲的连接功能，使用智能指针处理连接对象生命周期，设置了输出输入缓冲区减少了系统调用，提高了系统性能。
运行命令
服务端:./tcpepoll 192.168.240.139 5085
客户端:./client 192.168.240.139 5085
