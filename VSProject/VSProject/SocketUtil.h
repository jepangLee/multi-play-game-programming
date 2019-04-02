#pragma once
#include "UDPSocket.h"
#include "TCPSocket.h"
enum SocketAddressFamily {
	INET = AF_INET,
	INET6 = AF_INET6
};


class SocketUtil
{
public:
	static void SU_ReportError(const wchar_t *errorCode);
	static void SU_ReportError(const char *errorCode);
	static int SU_GetLastError();
private:

	UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
	TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);
};