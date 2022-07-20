#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class WaitState : public SkelDogState
{
private:
	float time_ = 0.0f;
public: // ������ & �Ҹ���
	WaitState() {};
	~WaitState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
