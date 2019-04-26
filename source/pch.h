#ifndef PCH_H
#define PCH_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN


#pragma comment(lib,"ws2_32")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <vector>
typedef int socklen_t;
//typedef char* receiveBufer_t;

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

//typedef void* receiveBufer_t;

typedef int SOCKET;
const int NO_ERROR = 0;
const int INVALID_SOCKET = -1;
const int WSAECONNRESET = ECONNRESET;
const int WSAEWOULDBLOCK = EAGAIN;
const int SOCKET_ERROR = -1;
#endif

using namespace std;
#endif