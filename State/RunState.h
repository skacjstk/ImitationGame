#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class RunState : public SkelDogState
{
private:
	float distance;
	float actorScale;
public: // ������ & �Ҹ���
	RunState() {};
	~RunState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
