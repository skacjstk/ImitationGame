#pragma once
class LittleGhostState	// �갡 ��ǻ� State ��ü�� ����
{
public: // ������ & �Ҹ���
	LittleGhostState() {};
	~LittleGhostState() {};
public:
	virtual void SwitchState(class LittleGhost& actor) {};
	virtual void Action(class LittleGhost& actor) {};
	virtual void Enter(class LittleGhost& actor) {};
};
