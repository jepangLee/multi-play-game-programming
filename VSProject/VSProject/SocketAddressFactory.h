#pragma once
#include "pch.h"
#include "SocketAddress.h"
class SocketAddressPactory {

public:
	static SocketAddressPtr CreateIPv4FromString (const string& inString)
	{
		auto pos = inString.find_last_of(":");
		string host, service;
		if (string::npos != pos) {
			host = inString.substr(0, pos);
			service = inString.substr(pos + 1);
		}
		else {
			// ��Ʈ�� �������� �ʾ��� -> default ���
			host = inString;
			service = "0";
		}
		addrinfo hint;
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;

		addrinfo* result = nullptr;
		int error = getaddrinfo(host.c_str(), service.c_str, &hint, &result);
		addrinfo* initResult = result;

		if (NULL != error && nullptr != result) {
			freeaddrinfo(initResult);
			return nullptr;
		}

		while (NULL == (result->ai_addr && result->ai_next) ) {
			result = result->ai_next;
		}

		if (NULL == result->ai_addr) {
			freeaddrinfo(initResult);
			return nullptr;
		}

		auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

		freeaddrinfo(initResult);
		return toRet;
	}
};