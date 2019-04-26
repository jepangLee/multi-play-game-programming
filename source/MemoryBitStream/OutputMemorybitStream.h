#ifndef OUTPUT_MEMORY_BIT_STREAM_H
#define OUTPUT_MEMORY_BIT_STREAM_H

#include "../pch.h"

class OutputMemorybitStream
{
public:
	OutputMemorybitStream() :mBuffer(nullptr), mBitHead(0), mBitCapacity(0)
	{
		ReallocBuffer(256);
	}
	~OutputMemorybitStream() { free(mBuffer); }


	void WriteBits(const void* inData, uint32_t inBitCount);
	void WriteBits(uint8_t inData, size_t inBitCount);

	template<class T>
	void WriteByte(T inData, size_t inByteCount);

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetBitLength() const { return mBitHead; }
	uint32_t GetBitCapacity() const { return mBitCapacity; }


private:
	void ReallocBuffer(uint32_t inNewByteCapacity);

	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};

#endif

