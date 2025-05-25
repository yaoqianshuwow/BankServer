// class Connection
// {
// private:
//     EventLoop *loop_;               // Connection对应的事件循环，在构造函数中传入。 
//     Socket *clientsock_;             // 与客户端通讯的Socket。
//     Channel *clientchannel_;     // Connection对应的channel，在构造函数中创建。
// public:
//     Connection(EventLoop *loop,Socket *clientsock);
//     ~Connection();
// };

#include"Connection.h"
Connection::Connection(EventLoop *loop,Socket *clientsock):loop_(loop),clientsock_(clientsock),clientchannel_(new Channel(loop_,clientsock_->fd()))
{
    //clientchannel_=new Channel(loop_,clientsock_->fd());   // 这里new出来的对象没有释放，这个问题以后再解决。
    clientchannel_->useet();                 // 客户端连上来的fd采用边缘触发。
    
    clientchannel_->enablereading();   // 让epoll_wait()监视clientchannel的读事件
    clientchannel_->setreadcallback(bind(&Connection::onmessage,this));
    clientchannel_->setwritecallback(bind(&Connection::writecallback,this));
    clientchannel_->setclosecallback(bind(&Connection::closecallback,this));
    clientchannel_->seterrorcallback(bind(&Connection::errorcallback,this));
}
Connection::~Connection(){
cout<<"调用了析构函数"<<endl;
// delete clientchannel_;
// delete clientsock_;??

}
string Connection::ip()const{

return clientsock_->ip();
}
uint16_t Connection::port()const{
return clientsock_->port();
}
int Connection::fd()const{
    return clientsock_->fd();
}
void Connection::closecallback(){
    //closecallback_(shared_from_this);//Ch->移除con关闭->eco printf -> tcp删除conns
    printf("1connection(eventfd=%d) disconnected.\n", this->fd());
    close(this->fd());            // 关闭客户端的 fd。
    closecallback_(shared_from_this());
    //delete this; 不要delete 也不能
} 
void Connection::errorcallback(){
    errorcallback_(shared_from_this());
    printf("2connection(eventfd=%d) disconnected.\n", this->fd());
    close(this->fd());            // 关闭客户端的 fd。
    delete this;
}
void Connection::setclosecallback(std::function<void(spConnection)>fn){
 closecallback_=fn;
}
void Connection::seterrorcallback(std::function<void(spConnection)>fn){
  errorcallback_=fn;

}

void Connection::onmessage(){

char buffer[1024];


while (true) {             // 由于使用非阻塞 IO，一次读取 buffer 大小数据，直到全部的数据读取完毕。
    bzero(&buffer, sizeof(buffer));
    ssize_t nread = read(this->fd(), buffer, sizeof(buffer));    // 这行代码用了 read()，也可以用 recv()，一样的，不要纠结。
    if (nread > 0) {      // 成功的读取到了数据。
        
        inputbuffer_.append(buffer,nread);
       
    } 
    
    else if (nread == -1 && errno == EINTR) { // 读取数据的时候被信号中断，继续读取。
        continue;
    } else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { // 全部的数据已读取完毕。

         while(true){
            if(inputbuffer_.size()<4)break;
            int len;
            memcpy(&len,inputbuffer_.data(),4);
            //cout<<len<<endl;
            if(inputbuffer_.size()<len+4)break;
            string str(inputbuffer_.data()+4,len);
            ts=Timestamp::now();
            printf("recv(eventfd=%d): %s", fd(), str.c_str());
     
            onmessagecallback_(shared_from_this(),str);
        // 接收信息(读事件)：在subloop线程里运行，1、onmessage分包
        // 计算过程（和读事件同时调用）:在工作线程里运行,1、简单计算（数据库写入也可以），2、send函数写入发送缓冲区开启写事件监听，3、开启监听写事件。
        // 回应信息(写事件，内核发送缓冲区空就会触发):subloop线程里运行,1、writecallback函数发送数据，2、outbuffer空就关闭监听。
            inputbuffer_.erase(0,len+4);
            
            
        }
        
        break;
    } else if (nread == 0) {  // 客户端连接已断开。
        
        closecallback();
        break;
    }
}


}
void Connection:: writecallback(){
    
    int writen=::send(fd(),outputbuffer_.data(),outputbuffer_.size(),0);    // 尝试把outputbuffer_中的数据全部发送出去。
    if (writen>0) outputbuffer_.erase(0,writen);                                        // 从outputbuffer_中删除已成功发送的字节数。

    // 如果发送缓冲区中没有数据了，表示数据已发送完成，不再关注写事件。
    if (outputbuffer_.size()==0) clientchannel_->disablewriting();    

}
void Connection::setonmessagecallback( std::function<void(spConnection,std::string)>fn){

    onmessagecallback_=fn;
 }
void Connection::send(const char *data,size_t size){
    if(loop_->isinloopthread()){
    printf("send()在事件循环里操作buffer\n");
    outputbuffer_.append(data,size);
    clientchannel_->enablewriting();
    }
    else{

    printf("send()不在事件循环里操作buffer\n");
    string str(&data[4],size-4);
    cout<<"服务端回应报文:"<<str<<endl;
    char*message=new char[size];
    memcpy(message,data,size);
    loop_->queueinloopthread(bind(&Connection::sendinloop,this,message,size));
    }

}
void Connection::sendinloop(const char *data,size_t size)
{
    outputbuffer_.append(data,size);    // 把需要发送的数据保存到Connection的发送缓冲区中。
    clientchannel_->enablewriting();    // 注册写事件
    string str(&data[4],size-4);
    cout<<"服务端回应报文:"<<str<<endl;
    delete data;
}
int Connection::laststamp(){


    return ts.toint();
}