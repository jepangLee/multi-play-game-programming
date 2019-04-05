#include "pch.h"
#include "SocketUtil.h"


TCPSocket::~TCPSocket()
{
}

int TCPSocket::Connect(const SocketAddress & inAddress)
{
	int err = connect(this->mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err >= 0)
		return NO_ERROR;

	SocketUtil::SU_ReportError(L"TCPSocket::Connet");
	return -SocketUtil::SU_GetLastError();
}

int TCPSocket::Bind(const SocketAddress & inToAddress)
{
	

}

int TCPSocket::Listen(int inBackLog)
{
	int err = listen(this->mSocket, inBackLog);

	if (err >= 0)
		return NO_ERROR;

	SocketUtil::SU_ReportError(L"TCPSocket::Listen");
	return -SocketUtil::SU_GetLastError();
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
