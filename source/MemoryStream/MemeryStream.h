#ifndef MEMORY_STREAM_h
#define MEMORY_STREAM_h

#include "../pch.h"

class OutputMemoryStream
{
public:
	OutputMemoryStream() : mBuffer(nullptr), mHead(0), mCapacity(0)
	{ ReallocBuffer(32); }
	~OutputMemoryStream() { free(mBuffer); }

	const char* GetBugfferPtr() const { return mBuffer; }
	uint32_t GetLength() const { return mHead; }

	template<class T>
	void Write(const T& inData)
	{
		static_assert(
			is_arithmetic<T>::value ||
			is_enum<T>::value,
			"Genertic Wirte only supports primitive data type");

		Write(&inData, sizeof(inData));
	}
	void Write(const void* inData, size_t inByteCount);

private:
	void ReallocBuffer(uint32_t inNewLength);

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};

class InputMemeryStream
{
public:
	InputMemeryStream(char* inBuffer, uint32_t inByteCount)
		: mBuffer(inBuffer), mCapacity(inByteCount), mHead(0)
	{ }

	~InputMemeryStream() { free(mBuffer); }

	uint32_t GetRemainingDataSize() const { return mCapacity - mHead; }

	template<class T>
	void Read(T& outData) {
		Read(&outData, sizeof(outData));
	}
	void Read(void* OutData, uint32_t inByteCount);

private:

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};

#endif


