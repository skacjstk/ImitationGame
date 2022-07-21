#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
class JumpState : public SkelDogState
{
public: // 持失切 & 社瑚切
	JumpState() {};
	~JumpState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
