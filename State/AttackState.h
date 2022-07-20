#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class AttackState : public SkelDogState
{
private:
	Vector2 firstAttackPos = Vector2(0.0f,0.0f);
	float time_ = 0.0f;
	float dX = 0.0f;
public: // ������ & �Ҹ���
	AttackState() {};
	~AttackState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
