网络银行(在linux环境下，用自己的ip运行）
1、利用面向对象思想封装了epoll和socket，增强了代码的可读性，实现了高并发接受请求报文。
2、buffer类设置了输出输入缓冲区减少了系统调用，提高了系统性能。
3、报文头添加报文，长度解决了粘包问题。
4、使用eventfd添加到epoll方式，实现了线程间同步与通信。
5、 采用了线程池技术，给每个事件循环都分配了一个线程，还给数据的计算工作也分配了线程，提高了系统的对cpu的利用效率。
6、使用map红黑树，实现了定时清理空闲的连接功能，避免了系统支援被长时间占用。
7、使用智能指针处理连接对象生命周期，避免了内存泄漏和误操作，提高了代码的可维护性。
8、使用互斥锁和消息队列，实现了临界资源的互斥使用，保证了线程安全。
服务端:./tcpepoll 192.168.240.139 5085
客户端:./client 192.168.240.139 5085
