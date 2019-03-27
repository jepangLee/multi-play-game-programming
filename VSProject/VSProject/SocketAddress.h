#pragma once

#include "pch.h"

class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(PCWSTR inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);
	size_t GetSize() const;

private:
	friend class UDPSocket;
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn();
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;