#pragma once
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
class RunStateBWS : public BigWhiteSkelState
{
private:
	float distance = 0.0f;
	float actorScale = 0.0f;
	Vector2 beforePos = Vector2(0.0f, 0.0f);
public: // ������ & �Ҹ���
	RunStateBWS() {};
	~RunStateBWS() {};
public:
	void SwitchState(class BigWhiteSkel& actor) override;
	void Action(class BigWhiteSkel& actor) override;
	void Enter(class BigWhiteSkel& actor) override;
};
