#pragma once
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
class IdleStateBWS : public BigWhiteSkelState
{
private:
	float distance;
	float actorScale;
public: // 持失切 & 社瑚切
	IdleStateBWS() {};
	~IdleStateBWS() {};
public:
	void SwitchState(class BigWhiteSkel& actor) override;
	void Action(class BigWhiteSkel& actor) override;
	void Enter(class BigWhiteSkel& actor) override;
};
