#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include "SocketAddress.h"

class SocketUtil;

class UDPSocket
{
public:
	
	int Bind(const SocketAddress& inBindAddress);
	int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
	int ReceiveFrom(void* inBuffer, int inMaxLengh, SocketAddress& outFrom);
	
	~UDPSocket();
private:
	friend class SocketUtil;

	UDPSocket() = delete;
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	
	SOCKET mSocket;
};

using UDPSocketPtr = shared_ptr<UDPSocket>;

#endif // UDPSOCKET