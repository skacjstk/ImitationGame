#pragma once
#include "Object/Enemy/LittleGhost.h"
#include "State/LittleGhostState/LittleGhostState.h"
class MoveStateLG : public LittleGhostState
{
private:
	float distance = 0.0f;
	float actorScale = 0.0f;
	float radian = 0.0f;
	Vector2 beforePos = Vector2(0.0f, 0.0f);
public: // 持失切 & 社瑚切
	MoveStateLG() {};
	~MoveStateLG() {};
public:
	void SwitchState(class LittleGhost& actor) override;
	void Action(class LittleGhost& actor) override;
	void Enter(class LittleGhost& actor) override;
};
