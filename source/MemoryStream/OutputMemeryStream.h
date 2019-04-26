#ifndef OUTPUT_MEMORY_STREAM_h
#define OUTPUT_MEMORY_STREAM_h

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
	void Write(const T& inData);
	void Write(const void* inData, size_t inByteCount);

private:
	void ReallocBuffer(uint32_t inNewLength);

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};

template<class T>
void OutputMemoryStream::Write(const T& inData)
{
	static_assert(
		is_arithmetic<T>::value ||
		is_enum<T>::value,
		"Genertic Wirte only supports primitive data type");

	Write(&inData, sizeof(inData));
}

#endif


