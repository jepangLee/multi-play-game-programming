#ifndef	SOCKET_UTIL_H
#define SOCKET_UTIL_H

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
	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);
	static fd_set* FillSetFromVector(fd_set& outset, const vector<TCPSocketPtr>* inSockets);
	static fd_set* FillVectorFromSet(
		vector<TCPSocketPtr>* outset, 
		const vector<TCPSocketPtr>* inSocket, 
		const fd_set& inSet);
	static int Select(
		const vector<TCPSocketPtr>* inReadSet, vector<TCPSocketPtr> outReadSet,
		const vector<TCPSocketPtr>* inWriteSet, vector<TCPSocketPtr> outWriteSet,
		const vector<TCPSocketPtr>* inExceptSet, vector<TCPSocketPtr> outExceptSet
	);

};

#endif // SOCKETUTIL_H

