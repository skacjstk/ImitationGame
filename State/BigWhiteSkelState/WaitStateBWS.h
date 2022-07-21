#pragma once
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
class WaitStateBWS : public BigWhiteSkelState
{
private:
	float time_ = 0.0f;
public: // 持失切 & 社瑚切
	WaitStateBWS() {};
	~WaitStateBWS() {};
public:
	void SwitchState(class BigWhiteSkel& actor) override;
	void Action(class BigWhiteSkel& actor) override;
	void Enter(class BigWhiteSkel& actor) override;
};
