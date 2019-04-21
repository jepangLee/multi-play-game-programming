#include "pch.h"
#include "SocketAddressFactory.h"

SocketAddressPtr SocketAddressPactory::CreateIPv4FromString(const string & inString)
{
	auto pos = inString.find_last_of(":");
	string host, service;
	if (string::npos != pos) {
		host = inString.substr(0, pos);
		service = inString.substr(pos + 1);
	}
	else {
		// 포트가 정해지지 않았음 -> default 사용
		host = inString;
		service = "0";
	}
	addrinfo hint;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	addrinfo* result = nullptr;
	int error = getaddrinfo((PCSTR)&host, (PCSTR)&service, &hint, &result);
	addrinfo* initResult = result;

	if (0 != error && nullptr != result) {
		freeaddrinfo(initResult);
		return nullptr;
	}

	while (!result->ai_addr && result->ai_next) {
		result = result->ai_next;
	}

	if (!result->ai_addr) {
		freeaddrinfo(initResult);
		return nullptr;
	}

	auto toRet = make_shared<SocketAddress>(*result->ai_addr);

	freeaddrinfo(initResult);
	return toRet;
}
