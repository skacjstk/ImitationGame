#pragma once
class BigWhiteSkelState	// �갡 ��ǻ� State ��ü�� ����
{
public: // ������ & �Ҹ���
	BigWhiteSkelState() {};
	~BigWhiteSkelState() {};
public:
	virtual void SwitchState(class BigWhiteSkel& actor) {};
	virtual void Action(class BigWhiteSkel& actor) {};
	virtual void Enter(class BigWhiteSkel& actor) {};
};
