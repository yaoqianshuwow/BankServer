#include"EchoServer.h"


// class EchoServer
// {
// private:
//     TcpServer tcpserver_;

// public:
//     EchoServer(const std::string &ip,const uint16_t port);
//     ~EchoServer();

//     void Start();                // 启动服务。

//     void HandleNewConnection(Connection *conn);     // 处理新客户端连接请求，在TcpServer类中回调此函数。
//     void HandleClose(Connection *conn);                      // 关闭客户端的连接，在TcpServer类中回调此函数。 
//     void HandleError(Connection *conn);                       // 客户端的连接错误，在TcpServer类中回调此函数。
//     void HandleMessage(Connection *conn,std::string message);     // 处理客户端的请求报文，在TcpServer类中回调此函数。
//     void HandleSendComplete(Connection *conn);        // 数据发送完成后，在TcpServer类中回调此函数。
//     // void HandleTimeOut(EventLoop *loop);                    // epoll_wait()超时，在TcpServer类中回调此函数。
// };
EchoServer::EchoServer(const std::string &ip,const uint16_t port,unsigned short IOthread,unsigned short  workthread):tcpserver_(ip,port,IOthread),threadpool_(workthread,"WORK")
{


tcpserver_.setnewconnectioncb(std::bind(&EchoServer::HandleNewConnection, this, std::placeholders::_1));
tcpserver_.setcloseconnectioncb(std::bind(&EchoServer::HandleClose, this, std::placeholders::_1));
tcpserver_.seterrorconnectioncb(std::bind(&EchoServer::HandleError, this, std::placeholders::_1));
tcpserver_.setonmessagecb(std::bind(&EchoServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
tcpserver_.setsendcompletecb(std::bind(&EchoServer::HandleSendComplete, this, std::placeholders::_1));


}
EchoServer::~EchoServer(){

}
void EchoServer::Start(){
tcpserver_.start();


}
void EchoServer::HandleNewConnection(spConnection conn){

  std::cout << "New Connection Come in." << std::endl;

}

void EchoServer::HandleClose(spConnection conn){

std::cout << "EchoServer conn closed." << std::endl;

}

void EchoServer::HandleError(spConnection conn){

std::cout << "EchoServer conn error." << std::endl;
}

#include <sys/syscall.h> // 定义 SYS_gettid
void EchoServer::HandleMessage(spConnection conn,std::string message){
  
 if(threadpool_.idnum()==0){
   ONMessage(conn, message);
 }
 else{

 threadpool_.addtask([this, conn, message]() { ONMessage(conn, message); });
 }

}

void EchoServer::ONMessage(spConnection conn,std::string message){
  
  cout << "......正在进行数据库操作（mysql or redis),请耐心等待"<<endl;
  cout<<"等待发送数据"<<message<<endl;
  cout<<"Thread ID (TID) " <<syscall(SYS_gettid)<< std::endl;

  int len=message.size();
  string data((char*)(&len),4);
  data.append(message);
  //cout<<"等待发送数据"<<message<<endl;
  //.....................数据库操作（mysql or redis)
  //sleep(1);
  
  cout<<"计算结束，正在通知事件循环，发送回应报文"<<endl;
  // send(conn->fd(),data.data()+4,len,0);

   conn->send(data.data(),data.size());
}
void EchoServer::HandleSendComplete(spConnection conn){


 std::cout << "Message send complete." << std::endl;


}














