#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
class WaitState : public SkelDogState
{
private:
	float time_ = 0.0f;
public: // 持失切 & 社瑚切
	WaitState() {};
	~WaitState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
