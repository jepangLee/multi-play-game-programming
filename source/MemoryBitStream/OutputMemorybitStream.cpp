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

	uint32_t byteOffset = mBitHead >> 3; //��Ʈ ��带 8�� ����
	uint32_t bitOffset = mBitHead & 0x7; //8�� ���� ������

	//���� ó�� ���� 1byte���� �� bit�� ������ ���
	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask)
		| (inData << bitOffset);


	//����� 1byte ���� �� ��Ʈ�� �����ֳ� ���
	uint32_t bitsFreeThisByte = 8 - bitOffset;

	//������ ���ڸ��� ���� ����Ʈ�� �ѱ�
	if (bitsFreeThisByte < inBitCount) {
		// ���� ����Ʈ�� ������ ��Ʈ�� ���
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}