#pragma once
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
class JumpStateBWS : public BigWhiteSkelState
{
public: // ������ & �Ҹ���
	JumpStateBWS() {};
	~JumpStateBWS() {};
public:
	void SwitchState(class BigWhiteSkel& actor) override;
	void Action(class BigWhiteSkel& actor) override;
	void Enter(class BigWhiteSkel& actor) override;
};
