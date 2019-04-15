#ifndef ROBOCAT_H
#define ROBOCAT_H
#include "GameObject.h"

class RoboCat : public GameObject
{
public:
	RoboCat() : mHealth(10), mMeowCount(3) {}

private:
	int32_t mHealth;
	int32_t mMeowCount;
};


#endif // ROBOCAT_H