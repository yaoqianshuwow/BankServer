all:client tcpepoll

client:client.cpp
	g++  -o client client.cpp
tcpepoll:tcpepoll.cpp
	g++  -o tcpepoll tcpepoll.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Acceptor.cpp Connection.cpp

clean:
	rm -f client tcpepoll