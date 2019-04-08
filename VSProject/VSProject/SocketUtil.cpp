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

//void DoUDPLoop() {
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
//}