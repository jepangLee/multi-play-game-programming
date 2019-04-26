#include "OutputMemorybitStream.h"

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

	uint32_t byteOffset = mBitHead >> 3; //비트 헤드를 8로 나눔
	uint32_t bitOffset = mBitHead & 0x7; //8에 대한 나머지

	//현재 처리 중인 1byte에서 몇 bit를 남길지 계산
	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask)
		| (inData << bitOffset);


	//기록할 1byte 내에 몇 비트가 남아있나 계산
	uint32_t bitsFreeThisByte = 8 - bitOffset;

	//공간이 모자르면 다음 바이트에 넘김
	if (bitsFreeThisByte < inBitCount) {
		// 다음 바이트에 나머지 비트를 기록
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}