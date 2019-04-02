#include "pch.h"
#include "TCPSocket.h"


TCPSocket::~TCPSocket()
{
}

int TCPSocket::Connect(const SocketAddress & inAddress)
{
	return 0;
}

int TCPSocket::Bind(const SocketAddress & inToAddress)
{
	return 0;
}

int TCPSocket::Listen(int inBackLog)
{
	return 0;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress & inFromAddress)
{
	return TCPSocketPtr();
}


int TCPSocket::Send(const void * inData, int inLen)
{
	return 0;
}

int TCPSocket::Receive(void * inBuffer, int inLen)
{
	return 0;
}
