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
#if defined( WIN32 )
	return WSAGetLastError();
#else
	return errno;
#endif

}

//UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
//{
//	SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
//	if (INVALID_SOCKET != s)
//		return UDPSocketPtr(new UDPSocket(s));
//
//	
//}