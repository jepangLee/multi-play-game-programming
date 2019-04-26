#include "RoboCat.h"
#include "GameObject.h"

void RoboCat::Read(InputMemeryStream inStream)
{
	inStream.Read(mHealth);
	inStream.Read(mMeowCount);
	//mHomeBase
	inStream.Read(mName, 128);
	//mMiceIndices
}

void RoboCat::Write(OutputMemoryStream inStream) const
{
	inStream.Write(mHealth);
	inStream.Write(mMeowCount);
	//mHomeBase
	inStream.Write(mName, 128);
	//mMiceIndices
}

void RoboCat::SendRoboCat(int inSocket, OutputMemoryStream stream) {
	Write(stream);
	send(inSocket, stream.GetBugfferPtr(), stream.GetLength(), 0);
}

void RoboCat::ReceiveCRoboCat(int inSocket)
{
	char* temporaryBuffer = static_cast<char*>(malloc(MaxPacketSize));
	size_t receivedByteCount = recv(inSocket, temporaryBuffer, MaxPacketSize, 0);

	if (receivedByteCount > 0) {
		InputMemeryStream stream(temporaryBuffer, static_cast<uint32_t>(receivedByteCount));
		Read(stream);
	}
	else free(temporaryBuffer);
}