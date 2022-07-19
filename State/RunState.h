#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class RunState : public SkelDogState
{
private:
	float duringTime = 0.0f;
public: // 持失切 & 社瑚切
	RunState() {};
	~RunState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
};
