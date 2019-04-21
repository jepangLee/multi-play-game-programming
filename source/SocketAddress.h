#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H


#include "pch.h"

class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);
	SocketAddress() = delete;
	size_t GetSize() const;

private:
	friend class UDPSocket;
	friend class TCPSocket;
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn();
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;

#endif