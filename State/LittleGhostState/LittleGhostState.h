#pragma once
class LittleGhostState	// 얘가 사실상 State 객체의 원형
{
public: // 생성자 & 소멸자
	LittleGhostState() {};
	~LittleGhostState() {};
public:
	virtual void SwitchState(class LittleGhost& actor) {};
	virtual void Action(class LittleGhost& actor) {};
	virtual void Enter(class LittleGhost& actor) {};
};
