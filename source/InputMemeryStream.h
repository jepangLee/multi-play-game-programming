#ifndef INPUT_MEMORY_STREAM_h
#define INPUT_MEMORY_STREAM_h

#include "pch.h"
class InputMemeryStream
{
public:
	InputMemeryStream(char* inBuffer, uint32_t inByteCount)
		: mCapacity(inByteCount), mHead(0) 
	{
		mBuffer = static_cast<char*>(malloc(sizeof(char) * mCapacity));
		memcpy(mBuffer, inBuffer, mCapacity);
	}

	~InputMemeryStream()
	{
		free(mBuffer);
	}

	uint32_t GetRemainingDataSize() const { return mCapacity - mHead; }

	template<class T>
	void Read(T& outData) { Read(&outData, sizeof(outData)); }
	void Read(void* OutData, uint32_t inByteCount);

private:

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};

#endif