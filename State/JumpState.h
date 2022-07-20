#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class JumpState : public SkelDogState
{
public: // ������ & �Ҹ���
	JumpState() {};
	~JumpState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
