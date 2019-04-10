#include "pch.h"
#include "SocketAddress.h"

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
	this->GetAsSockAddrIn()->sin_family = AF_INET;
	this->GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
	this->GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(PCWSTR inAddress, uint16_t inPort)
{
	this->GetAsSockAddrIn()->sin_family = AF_INET;
	InetPton(AF_INET, inAddress, &GetAsSockAddrIn()->sin_addr);
	this->GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr & inSockAddr)
{
	memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}

size_t SocketAddress::GetSize() const
{
	return sizeof(sockaddr);
}

sockaddr_in * SocketAddress::GetAsSockAddrIn()
{
	return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}