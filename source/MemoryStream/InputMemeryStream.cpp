#include "InputMemeryStream.h"

void InputMemeryStream::Read(void* OutData, uint32_t inByteCount)
{
	if (inByteCount <=
				static_cast<uint32_t>(_msize(mBuffer + mHead))) {
		memcpy(OutData, mBuffer + mHead, inByteCount);
		mHead += inByteCount;
	}

	
}