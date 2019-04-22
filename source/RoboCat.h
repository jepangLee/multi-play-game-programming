#ifndef ROBOCAT_H
#define ROBOCAT_H
#include "GameObject.h"
#include "InputMemeryStream.h"
#include "OutputMemeryStream.h"

const int MaxPacketSize = 1470;

class RoboCat : public GameObject
{
public:
	RoboCat() : mHealth(10), mMeowCount(3), mHomeBase(0) 
	{
		mName[0] = '/0';
	}

	void Read(InputMemeryStream inStream);
	void Write(OutputMemeryStream inStream) const;

	void SendRoboCat(int inSocket, OutputMemeryStream stream);
	void ReceiveCRoboCat(int inSocket);
	
private:
	int32_t			mHealth;
	int32_t			mMeowCount;
	GameObject*		mHomeBase;
	char			mName[128];
	vector<int32_t>	mMiceIndices;
};


#endif