#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
class IdleState : public SkelDogState
{
private:
	float distance;
	float actorScale;
public: // 持失切 & 社瑚切
	IdleState() {};
	~IdleState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
