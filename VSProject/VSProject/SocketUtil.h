#pragma once
#include "UDPSocket.h"

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
};