#pragma once
#include <xstring>
#include "pch.h"
#include "SocketAddress.h"

class SocketAddressPactory {

public:
	static SocketAddressPtr CreateIPv4FromString(const string& inString);
	
};