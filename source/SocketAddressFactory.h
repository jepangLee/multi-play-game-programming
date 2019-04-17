#ifndef SOCKET_ADDRESS_FACTORY_H
#define SOCKET_ADDRESS_FACTORY_H

#include "pch.h"
#include "SocketAddress.h"

class SocketAddressPactory {

public:
	static SocketAddressPtr CreateIPv4FromString(const string& inString);

};

#endif 

