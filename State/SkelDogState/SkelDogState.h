#pragma once
class SkelDogState	// 얘가 사실상 State 객체의 원형
{
public:
//	static AttackState attack_;
public: // 생성자 & 소멸자
	SkelDogState() {};
	~SkelDogState() {};
public:
	virtual void SwitchState(class SkelDog& actor) {};
	virtual void Action(class SkelDog& actor) {};
	virtual void Enter(class SkelDog& actor) {};
};
