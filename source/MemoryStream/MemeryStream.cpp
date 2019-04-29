#include "MemeryStream.h"

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	const char* temp = mBuffer;

	mBuffer = static_cast<char*>(realloc(mBuffer, inNewLength));
	if (mBuffer != temp) {
		mBuffer = static_cast<char*>(malloc(sizeof(char) * inNewLength));
		memcpy(mBuffer, temp, sizeof(temp));
	}

	mCapacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t resultHead = mHead + inByteCount;
	if (resultHead > mCapacity)
		ReallocBuffer(max(mCapacity * 2, resultHead));

	memcpy(mBuffer + mHead, inData, inByteCount);
	mHead = resultHead;
}

//-------------------------------------------------------------------------

void InputMemeryStream::Read(void* OutData, uint32_t inByteCount)
{
	if (inByteCount <=
		static_cast<uint32_t>(_msize(mBuffer + mHead))) {
		memcpy(OutData, mBuffer + mHead, inByteCount);
		mHead += inByteCount;
	}


}

