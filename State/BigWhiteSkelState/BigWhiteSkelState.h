#pragma once
class BigWhiteSkelState	// 얘가 사실상 State 객체의 원형
{
public: // 생성자 & 소멸자
	BigWhiteSkelState() {};
	~BigWhiteSkelState() {};
public:
	virtual void SwitchState(class BigWhiteSkel& actor) {};
	virtual void Action(class BigWhiteSkel& actor) {};
	virtual void Enter(class BigWhiteSkel& actor) {};
};
