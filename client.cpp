#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;
class ClientSocket{
private:

int sockfd;


public:

bool connects(const string& host ,const unsigned short port )
{
//1、创建socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
if (sockfd==-1)
  {
    perror("socket"); return false;
  }



//2、发起连接
struct hostent* h;    // 用于存放服务端IP的结构体。
  if ( (h = gethostbyname(host.c_str()))== 0 )  // 把字符串格式的IP转换成结构体。
  { 
    cout << "gethostbyname failed.\n" << endl;  return false;
  }



struct sockaddr_in servaddr;              // 用于存放服务端IP和端口的结构体。
memset(&servaddr,0,sizeof(servaddr));
servaddr.sin_family = AF_INET;    //协议
memcpy(&servaddr.sin_addr,h->h_addr,h->h_length); // IP地址。
servaddr.sin_port = htons(port);         // 端口。



if (::connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)  // 向服务端发起连接清求。
{ 
    perror("connect"); ::close(sockfd); return false; 
}
cout<<"wow连接成功"<<endl;
return true;
}



bool send(const string& data){

    int iret;
    if((iret=::send(sockfd,data.data(),data.size(),0))<=0){
      //cout<<"shibai"<<endl;
      perror("send");
      return false;
      }
    //cout<<"发送成功"<<endl;
    return true;

}
bool send(int len,const string& data){
  int iret;
  char data_[1024];
  memset(data_,0,sizeof(data_));
  memcpy(data_,&len,4);
  memcpy(data_+4,data.data(),len);
  //cout<<data_+4<<len<<endl;
  if((iret=::send(sockfd,data_,len+4,0))<=0){
    //cout<<"shibai"<<endl;
    perror("send");
    return false;
    }
  //cout<<"发送成功"<<endl;
  return true;

}
ssize_t recv( string &data,const size_t maxlen){
  data.clear();         // 清空容器。
  data.resize(maxlen);  // 设置容器的大小为maxlen。
  ssize_t iret;
  iret=::recv(sockfd,&data[0],maxlen, 0);
  //cout<<iret<<endl;
  return iret;
}

void close()
{
sockfd=-1;
::close(sockfd);
}

~ClientSocket(){
  close();
}
int getfd(){

  return sockfd;
}
};

void divistr(string &buffer){
while (1)
{
  if(buffer.size()<4)break;
  int len;
  memcpy(&len,buffer.data(),4);
  if(buffer.size()<len+4)break;
  string str(buffer.data()+4,len);
  cout<<"回应报文"<<str;
  //sleep(1);
  buffer.erase(0,len+4);

}
//cout<<buffer.size();
}
int main(int argc,char *argv[])
{  

 
  if (argc!=3)
  {
    cout << "Using:./client 服务端的IP 服务端的端口\nExample:./client 192.168.201.128 5085\n\n"; 
    return -1;
  }


  ClientSocket cs;

  if(!cs.connects(argv[1],(unsigned short)atoi(argv[2]))){
  //cout<<"拜拜"<<endl;
  return 0;
  }
  
  char buff[1024];


  for(int i=0;i<=80;i++){

  memset(buff,0,sizeof(buff));
  
  sprintf(buff,"数据测试进行时%d\n",i);

  int len=strlen(buff);
  
  
 
  //cout<<len<<endl;
 
   if(!cs.send(len,buff)){
    cout<<"发送失败"<<endl;
   return 0;}


  }
 
  for(int i=1;i<=100;i++){
    string buffer;
    string str;
    ssize_t size;
    size=cs.recv(str,1024);
    
    

  
    
    buffer.append(str.data(),size);
    divistr(buffer);
    
  }
    //divistr(buffer);

  sleep(5);
  
  cs.close();
  return 0;


}