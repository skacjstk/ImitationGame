#pragma once
#include "Object/Enemy/LittleGhost.h"
#include "State/LittleGhostState/LittleGhostState.h"
class AttackStateLG : public LittleGhostState
{
private:
	float time_ = 0.0f;
	float dX = 0.0f;
	float dY = 0.0f;
	float radian = 0.0f;
public: // 持失切 & 社瑚切
	AttackStateLG() {};
	~AttackStateLG() {};
public:
	void SwitchState(class LittleGhost& actor) override;
	void Action(class LittleGhost& actor) override;
	void Enter(class LittleGhost& actor) override;
};
