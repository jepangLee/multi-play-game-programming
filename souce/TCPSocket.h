#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "SocketAddress.h"

class SocketUtil;
class TCPSocket;

using TCPSocketPtr = shared_ptr<TCPSocket>;

class TCPSocket
{
public:
	~TCPSocket();
	int			 Connect(const SocketAddress& inAddress);
	int			 Bind(const SocketAddress& inBindAddress);
	int			 Listen(int inBackLog = 32);
	TCPSocketPtr Accept(SocketAddress& inFromAddress);
	int			 Send(const void* inData, int inLen);
	int			 Receive(void* inBuffer, int inLen);
private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};


#endif