#pragma once
#include "Object/Enemy/LittleGhost.h"
#include "State/LittleGhostState/LittleGhostState.h"
class IdleStateLG : public LittleGhostState
{
private:
	float distance = 0.0f;
	float actorScale = 0.0f;
public: // ������ & �Ҹ���
	IdleStateLG() {};
	~IdleStateLG() {};
public:
	void SwitchState(class LittleGhost& actor) override;
	void Action(class LittleGhost& actor) override;
	void Enter(class LittleGhost& actor) override;
};
