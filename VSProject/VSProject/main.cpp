#include "pch.h"
#include "SocketAddressFactory.h"

int main(void)
{
	SocketAddress *socketaddr = new SocketAddress(PCWSTR("10.156.145.118"), 80);
	
	return 0;
}