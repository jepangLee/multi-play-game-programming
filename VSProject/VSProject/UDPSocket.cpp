#include "pch.h"
#include "SocketUtil.h"

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int err = bind(this->mSocket, &inBindAddress.mSockAddr ,inBindAddress.GetSize() );

	if (err == 0)
		return NO_ERROR;

	SocketUtil::SU_ReportError(L"UDPSocket::bind");
	return SocketUtil::SU_GetLastError();
}

int UDPSocket::SendTo(const void * inData, int inLen, const SocketAddress & inTo)
{
	int byteSentCount = sendto(mSocket, static_cast<const char*>(inData),
								inLen, 0, &inTo.mSockAddr, inTo.GetSize());

	if (0 <= byteSentCount)
		return byteSentCount;

	//에러코드를 음수로 반환함
	SocketUtil::SU_ReportError(L"UDPSocket::SendTO");
	return SocketUtil::SU_GetLastError();

}

int UDPSocket::ReceiveFrom(void * inBuffer, int inMaxLengh, SocketAddress & outFrom)
{
	int fromLength = outFrom.GetSize();
	int readByteCount = recvfrom(mSocket, static_cast<char*>(inBuffer), inMaxLengh, 
									0, &outFrom.mSockAddr, &fromLength);
	if (readByteCount >= 0)
		return readByteCount;

	SocketUtil::SU_ReportError(L"UDPSocket::ReceiveFrom");
	return SocketUtil::SU_GetLastError();
}

UDPSocket::~UDPSocket()
{
	closesocket(this->mSocket);
}
