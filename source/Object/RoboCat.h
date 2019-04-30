#ifndef ROBOCAT_H
#define ROBOCAT_H

#include "../pch.h"
#include "GameObject.h"
#include "../MemoryStream/MemeryStream.h"

const int MaxPacketSize = 1470;

class RoboCat : public GameObject
{
public:
	RoboCat() : mHealth(10), mMeowCount(3), mHomeBase(0) 
	{
		mName[0] = '/0';
	}

	void Read(InputMemeryStream inStream);
	void Write(OutputMemoryStream inStream) const;

	void SendRoboCat(int inSocket, OutputMemoryStream stream);
	void ReceiveCRoboCat(int inSocket);
	
private:
	int32_t			mHealth;
	int32_t			mMeowCount;
	GameObject*		mHomeBase;
	char			mName[128];
	vector<int32_t>	mMiceIndices;
};


#endif