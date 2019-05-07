#include "MemorybitStream.h"

void OutputMemorybitStream::ReallocBuffer(uint32_t inNewBitCapacity)
{
	const char* temp = mBuffer;

	mBuffer = static_cast<char*>(realloc(mBuffer, inNewBitCapacity));
	if (mBuffer != temp) {
		mBuffer = static_cast<char*>(malloc(sizeof(char) * inNewBitCapacity));
		memcpy(mBuffer, temp, sizeof(temp));
	}

	mBitCapacity = inNewBitCapacity;
}


void OutputMemorybitStream::WriteBits(const void* inData, uint32_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);

	for (; inBitCount > 8; inBitCount -= 8) {
		WriteBits(*srcByte, 8);
		++srcByte;
	}

	if (inBitCount > 0)
		WriteBits(*srcByte, inBitCount);
}

void OutputMemorybitStream::WriteBits(uint8_t inData, size_t inBitCount)
{
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
	if (nextBitHead > mBitCapacity)
		ReallocBuffer(max(mBitCapacity * 2, nextBitHead));

	uint32_t byteOffset = mBitHead >> 3; 
	uint32_t bitOffset = mBitHead & 0x7; 

	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask)
		| (inData << bitOffset);
	
	uint32_t bitsFreeThisByte = 8 - bitOffset;

	
	if (bitsFreeThisByte < inBitCount) {
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}

void OutputMemorybitStream::WriteBytes(const void* inData, uint32_t inByteCount)
{
	WriteBits(inData, inByteCount << 3);
}

//-------------------------------------------------------------------------------------------

void InputMemorybitStream::ReadBits(void* outData, uint32_t inBitCount)
{
	uint8_t* destByte = reinterpret_cast<uint8_t*>(outData);
	while (inBitCount > 8)
	{
		ReadBits(*destByte, 8);
		++destByte;
		inBitCount -= 8;
	}
	if (inBitCount > 0)
	{
		ReadBits(*destByte, inBitCount);
	}
}

void InputMemorybitStream::ReadBits(uint8_t outData, size_t inBitCount)
{
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	outData = static_cast<uint8_t>(mBuffer[byteOffset]) >> bitOffset;

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount)
	{
		//we need another byte
		outData |= static_cast<uint8_t>(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
	}

	//don't forget a mask so that we only read the bit we wanted...
	outData &= (~(0x00ff << inBitCount));

	mBitHead += inBitCount;
}

void InputMemorybitStream::ReadBytes(void* outData, uint32_t inByteCount)
{
	ReadBits(outData, inByteCount << 3);
}

