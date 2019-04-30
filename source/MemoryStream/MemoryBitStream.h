#ifndef MEMORY_BIT_STREAM_H
#define MEMORY_BIT_STREAM_H

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

	void WriteBytes(const void* inData, uint32_t inByteCount);

	template<class T>
	void Write(T inData, size_t inBitCount = sizeof(T) * 8)
	{
		static_assert(
			is_arithmetic<T>::value ||
			is_enum<T>::value,
			"Generic write only supoort primitive data types");

		WriteBits(&inData, inBitCount);
	}
	
	template<class T>
	void Write(const vector<T>& inVector) {
		size_t elementCount = inVector.size();
		Write(elementCount);
		for (const T& element : inVector) {
			Write(element);
		}
	}
	
	template<>
	void Write(bool inData, size_t inBitCount) {
		WriteBits(inData, 1);
	}
	
	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetBitLength() const { return mBitHead; }
	uint32_t GetBitCapacity() const { return mBitCapacity; }


private:
	void ReallocBuffer(uint32_t inNewByteCapacity);

	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};

class InputMemorybitStream
{
public:
	InputMemorybitStream(char* inBuffer, uint32_t inBitCount)
		: mBitCapacity(inBitCount), mIsBufferOwner(false),
		  mBuffer(inBuffer), mBitHead(0)	
	{ }
	InputMemorybitStream(const InputMemorybitStream& inOther)
		: mBitCapacity(inOther.mBitCapacity), mIsBufferOwner(false),
		  mBitHead(inOther.mBitHead)
	{
		int byteCount = mBitCapacity * 8;
		mBuffer = static_cast<char*>(malloc(byteCount));
		memcpy(mBuffer, inOther.mBuffer, byteCount);
	}
	~InputMemorybitStream() { if (mIsBufferOwner) free(mBuffer); }


	void ReadBits(void* outData, uint32_t inBitCount);
	void ReadBits(uint8_t outData, size_t inBitCount);
	
	void ReadBytes(void* outData, uint32_t inByteCount);

	template<class T>
	void Read(T outData, size_t inBitCount = sizeof(T) * 8)
	{
		static_assert(
			is_arithmetic<T>::value ||
			is_enum<T>::value,
			"Generic write only supoort primitive data types");

		ReadBits(&outData, inBitCount);
	}
	template<class T>
	void Read(vector<T>& outVector) {
		size_t elementCount;
		Read(elementCount);
		outVector.resize(elementCount);
		for (T& element : outVector) {
			Read(element);
		}
	}
	template<>
	void Read(bool outData, size_t inBitCount) {
		ReadBits(outData, 1);	
	}

private:

	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
	bool mIsBufferOwner;
};
#endif

