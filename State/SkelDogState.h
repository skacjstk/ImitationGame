#pragma once
class SkelDogState	// �갡 ��ǻ� State ��ü�� ����
{
public:
//	static AttackState attack_;
public: // ������ & �Ҹ���
	SkelDogState();
	~SkelDogState() {};
public:
	virtual void SwitchState(class SkelDog& actor) {};
	virtual void Action(class SkelDog& actor) {};
};
