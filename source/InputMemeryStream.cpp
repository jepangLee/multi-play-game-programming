#include "InputMemeryStream.h"

void InputMemeryStream::Read(void* OutData, uint32_t inByteCount)
{
	memcpy(OutData, mBuffer + mHead, inByteCount);
	mHead += inByteCount;
}