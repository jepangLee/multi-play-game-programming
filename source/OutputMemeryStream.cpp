#include "OutputMemeryStream.h"

void OutputMemeryStream::ReallocBuffer(uint32_t inNewLength)
{
	const char* temp = mBuffer;

	mBuffer = static_cast<char*>(realloc(mBuffer, inNewLength));
	if (mBuffer != temp) {
		mBuffer = static_cast<char*>(malloc(sizeof(char) * inNewLength));
		memcpy(mBuffer, temp, sizeof(temp));
	}

	mCapacity = inNewLength;
}

void OutputMemeryStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t resultHead = mHead + inByteCount;
	if (resultHead > mCapacity)
		ReallocBuffer(max(mCapacity * 2, resultHead));

	memcpy(this->mBuffer + this->mHead, inData, inByteCount);
	mHead = resultHead;
}

template<class T>
inline void OutputMemeryStream::Write(const T& inData)
{
	static_assert(
		is_arithmetic<T>::value ||
		is_enum<T>::value,
		"Genertic Wirte only supports primitive data type");
	
	this->Write(&inData, sizeof(inData));
}