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
TcpServer::TcpServer(const std::string &ip,const uint16_t port,unsigned short  threadnum):pool_(threadnum,"IO"){

acceptor_=new Acceptor(&loop_,ip,port);

acceptor_->setnewconnectioncb(bind(&TcpServer::newconnection,this,placeholders::_1));    
loop_.settimeout(true,5);
 for(int i=0;i<pool_.idnum();i++){

   subloop_.push_back(std::make_unique<EventLoop>());
   auto task = std::make_shared<std::packaged_task<void()>>(
    std::bind(&EventLoop::run, subloop_[i].get())
   );
   
   //
   pool_.addtask([task]() { (*task)(); });
   subloop_[i]->settimeout(false,10);
   //插入loop 为什么要用 uniptr呢？？？ 额 ，装逼好像是，没啥用
   
  // sleep(10);
 }

}
TcpServer::~TcpServer(){

//delete it.second;
}
void TcpServer::start(){

 loop_.run();

}

void TcpServer::newconnection(Socket *clientsock)
{  
    
   spConnection conn(new Connection(subloop_[clientsock->fd()%pool_.idnum()].get(),clientsock));   // 这里new出来的对象没有释放，这个问题以后再解决。
   printf ("newconnection(fd=%d,ip=%s,port=%d) ok.\n",clientsock->fd(),conn->ip().c_str(),conn->port());
   conn->setclosecallback(std::bind(&TcpServer::closeconnection,this,placeholders::_1));
   conn->seterrorcallback(std::bind(&TcpServer::errorconnection,this,placeholders::_1));
   conn->setonmessagecallback(std::bind(&TcpServer::onmessage,this,placeholders::_1,placeholders::_2));
   conns[conn->fd()]=conn;
   newconnectioncb_(conn);
   subloop_[clientsock->fd()%pool_.idnum()]->newconnewcton(conn->fd(),conn);
   subloop_[clientsock->fd()%pool_.idnum()]->settimeout(std::bind(&TcpServer::handeltimeout,this,placeholders::_1));

}
void TcpServer::closeconnection(spConnection conn){
   
   //conns.erase(conn->fd());
   closeconnectioncb_(conn);
   conns.erase(conn->fd());
   //计数器归零，加入不为零说明还有con在使用，不能析构
}
void TcpServer::errorconnection(spConnection conn){

   errorconnectioncb_(conn);
   conns.erase(conn->fd());
   //
}
#include <sys/syscall.h> // 定义 SYS_gettid
void TcpServer::onmessage(spConnection conn,string message){




onmessagecb_(conn,message);
}






void TcpServer::setnewconnectioncb(std::function<void(spConnection)> fn)
{
    newconnectioncb_=fn;
}

void TcpServer::setcloseconnectioncb(std::function<void(spConnection)> fn)
{
    closeconnectioncb_=fn;
}

void TcpServer::seterrorconnectioncb(std::function<void(spConnection)> fn)
{
    errorconnectioncb_=fn;
}

void TcpServer::setonmessagecb(std::function<void(spConnection,std::string &message)> fn)
{
    onmessagecb_=fn;
}

void TcpServer::setsendcompletecb(std::function<void(spConnection)> fn)
{
    sendcompletecb_=fn;
}

void TcpServer::settimeoutcb(std::function<void(EventLoop*)> fn)
{
    timeoutcb_=fn;
}

void TcpServer::handeltimeout(spConnection conn){
    lock_guard<mutex> lock{mt};
    conns.erase(conn->fd());
}