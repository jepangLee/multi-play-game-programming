#ifndef OUTPUT_MEMORY_STREAM_h
#define OUTPUT_MEMORY_STREAM_h

#include "pch.h"
class OutputMemeryStream
{
public:
	OutputMemeryStream() : mBuffer(nullptr), mHead(0), mCapacity(0)
	{
		ReallocBuffer(32);
	}
	~OutputMemeryStream() 
	{
		free(mBuffer);
	}

	const char* GetBugfferPtr() const { return mBuffer; }
	uint32_t GetLength() const { return mHead; }

	template<class T>
	void Write(const T& inData);
	void Write(const void* inData, size_t inByteCount);

private:
	void ReallocBuffer(uint32_t inNewLength);

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};

#endif


