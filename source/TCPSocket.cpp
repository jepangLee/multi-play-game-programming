#include "pch.h"
#include "SocketUtil.h"


TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Connect(const SocketAddress & inAddress)
{
	int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err >= 0)
		return NO_ERROR;

	SocketUtil::SU_ReportError(L"TCPSocket::Connet");
	return -SocketUtil::SU_GetLastError();
}

int TCPSocket::Bind(const SocketAddress & inBindAddress)
{
	int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());

	if (err == 0)
		return NO_ERROR;

	SocketUtil::SU_ReportError(L"UDPSocket::bind");
	return SocketUtil::SU_GetLastError();
}

int TCPSocket::Listen(int inBackLog)
{
	int err = listen(mSocket, inBackLog);

	if (err >= 0)
		return NO_ERROR;

	SocketUtil::SU_ReportError(L"TCPSocket::Listen");
	return -SocketUtil::SU_GetLastError();
}

TCPSocketPtr TCPSocket::Accept(SocketAddress & inFromAddress)
{
	int length = inFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

	if (newSocket != INVALID_SOCKET) 
		return TCPSocketPtr(new TCPSocket(newSocket));

	SocketUtil::SU_ReportError(L"TCPSocket::Accept");
	return nullptr;
}

int TCPSocket::Send(const void * inData, int inLen)
{
	int byteSentCount = send(mSocket, 
		static_cast<const char*>(inData), inLen, 0);
	
	if (byteSentCount >= 0)
		return byteSentCount;

	SocketUtil::SU_ReportError(L"TCPSocekt::Send");
	return -SocketUtil::SU_GetLastError();
}

int TCPSocket::Receive(void * inBuffer, int inLen)
{
	int byteReceivedCount = recv(mSocket, 
		static_cast<char*>(inBuffer), inLen, 0);

	if (byteReceivedCount >= 0)
		return byteReceivedCount;

	SocketUtil::SU_ReportError(L"TCPSoket::Receive");
	return -SocketUtil::SU_GetLastError();

}