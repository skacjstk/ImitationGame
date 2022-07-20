#pragma once
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
class RunState : public SkelDogState
{
private:
	float distance = 0.0f;
	float actorScale = 0.0f;
	Vector2 beforePos = Vector2(0.0f, 0.0f);
public: // 持失切 & 社瑚切
	RunState() {};
	~RunState() {};
public:
	void SwitchState(class SkelDog& actor) override;
	void Action(class SkelDog& actor) override;
	void Enter(class SkelDog& actor) override;
};
