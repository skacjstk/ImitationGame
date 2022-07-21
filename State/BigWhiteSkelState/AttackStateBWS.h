#pragma once
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
class AttackStateBWS : public BigWhiteSkelState
{
private:
	Vector2 firstAttackPos = Vector2(0.0f,0.0f);
	float time_ = 0.0f;
	float dX = 0.0f;
public: // 持失切 & 社瑚切
	AttackStateBWS() {};
	~AttackStateBWS() {};
public:
	void SwitchState(class BigWhiteSkel& actor) override;
	void Action(class BigWhiteSkel& actor) override;
	void Enter(class BigWhiteSkel& actor) override;
};
