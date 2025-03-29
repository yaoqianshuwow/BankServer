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
bool recv( string &data,const size_t maxlen){
  data.clear();         // 清空容器。
  data.resize(maxlen);  // 设置容器的大小为maxlen。
  int iret;
  if((iret=::recv(sockfd,&data[0],sizeof(data),1024)<=0))return false;
  return true;
}

void close()
{
sockfd=-1;
::close(sockfd);
}

~ClientSocket(){
  close();
}
};

 
int main(int argc,char *argv[])
{  

 
  if (argc!=3)
  {
    cout << "Using:./client 服务端的IP 服务端的端口\nExample:./client 192.168.240.139 5085\n\n"; 
    return -1;
  }


  ClientSocket cs;

  if(!cs.connects(argv[1],(unsigned short)atoi(argv[2]))){
  //cout<<"拜拜"<<endl;
  return 0;
  }
  char buff[1024];
//cout<<"1eionq12iodenq2io"<<endl;
//   sleep(10);

  // return 0;

  for(int i=1;i<=10;i++){

  memset(buff,0,sizeof(buff));
  printf("请输入发送内容 : ") ;
  cin>>buff;
  
  if(!cs.send(buff)){cout<<"发送失败"<<endl;
  return 0;}
  cout<<"客户端:"<<buff<<endl;


  string str;
  if(!cs.recv(str,1024)){
    cout<<"接收失败"<<endl;
  return 0;
  }
  cout<<"服务端："<<str<<endl;
   
  }
  cs.close();
  return 0;


}