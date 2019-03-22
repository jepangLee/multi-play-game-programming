#pragma once
#include "pch.h"

class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort) 
	{
		this->GetAsSockAddrIn()->sin_family = AF_INET;
		this->GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		this->GetAsSockAddrIn()->sin_port = htons(inPort);
	}
	SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

private:
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};

using SocketAddressPtr = shared_ptr<SocketAddress>;