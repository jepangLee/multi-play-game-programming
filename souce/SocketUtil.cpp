#include "pch.h"
#include "SocketUtil.h"

void SocketUtil::SU_ReportError(const wchar_t *errorCode)
{
	fputs((char *)errorCode, stderr);
	fputc('\n', stderr);
	exit(1);
}

void SocketUtil::SU_ReportError(const char *errorCode)
{
	fputs(errorCode, stderr);
	fputc('\n', stderr);
	exit(1);
}

int SocketUtil::SU_GetLastError() {
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif

}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET != s)
		return UDPSocketPtr(new UDPSocket(s));

	SocketUtil::SU_ReportError(L"SocketUtil::CreateUDPSocket");
	return nullptr;

}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET != s)
		return TCPSocketPtr(new TCPSocket(s));

	SocketUtil::SU_ReportError(L"SocketUtil::CreateUDPSocket");
	return nullptr;

}

fd_set* SocketUtil::FillSetFromVector(fd_set & outSet, const vector<TCPSocketPtr>* inSockets)
{
	if (inSockets) {
		FD_ZERO(&outSet);
		for (const TCPSocketPtr& socket : *inSockets) {
			FD_SET(socket->mSocket, &outSet);
		}
		return &outSet;
	}
	else return nullptr;
}

fd_set* SocketUtil::FillVectorFromSet(
	vector<TCPSocketPtr>* outSockets,
	const vector<TCPSocketPtr>* inSockets,
	const fd_set & inSet)
{
	if (inSockets->size() && outSockets->size()) {
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets) {
			if (FD_ISSET(socket->mSocket, &inSet)) {
				outSockets->push_back(socket);
			}
		}
	}
}

int SocketUtil::Select(
	const vector<TCPSocketPtr>* inReadSet, vector<TCPSocketPtr>* outReadSet,
	const vector<TCPSocketPtr>* inWriteSet, vector<TCPSocketPtr>* outWriteSet,
	const vector<TCPSocketPtr>* inExceptSet, vector<TCPSocketPtr>* outExceptSet)
{
	//TODO : �ϼ���..... ������ �Ѱ�����
}

void DoUDPLoop() {
	//	UDPSocketPtr mySock = SocketUtil::CreateUDPSocket(INET);
	//	mySock->SetNonBlockingMode(true);
	//
	//	bool gIsGamerunning = true;
	//
	//	while (gIsGamerunning) {
	//		char data[1500];
	//		SocketAddress socketAddress;
	//
	//		int bytesReceived = mySock->
	//			ReceiveFrom(data, sizeof(data), socketAddress);
	//		
	//		if (bytesReceived > 0)
	//			ProcessReceivedData(data, socketAddress);
	//	
	//		DoGameFrame()
	//	}
}

void DoTCPLoop() {
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddres(INADDR_ANY, 48000);
	if (listenSocket->Bind(receivingAddres) != NO_ERROR) return;

	vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	vector<TCPSocketPtr> readableSockets;

	bool gIsGameRunning = 1;
	while (gIsGameRunning) {
		if (!SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr))
			continue;

		for (const TCPSocketPtr& socket : readableSockets) {
			if (socket == listenSocket) {
				//������ ���Ͽ��� �� ������ �޴´�.
				SocketAddress newClientAddress;
				auto newSocket = listenSocket->Accept(newClientAddress);
				readBlockSockets.push_back(newSocket);
				//ProcessNewClient(newSocket, newClientAddresss)
			}
			else {
				//�Ϲ� �����̹Ƿ� �����͸� �����Ѵ�.
				//char segment[GOOD_SEGMENT_SIZE];
				//int dataReceived = 
				//		socket->Receive(segment, GOOD_SEGMENT_SIZE);
				//if(dataReceived > 0)
				//	ProcessDataFromClient(socket, segment, dataReceived);
			}
		}
	}
}