#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class IdleState : public SkelDogState
{
private:
	float duringTime = 0.0f;
public: // ������ & �Ҹ���
	IdleState() {};
	~IdleState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
};
