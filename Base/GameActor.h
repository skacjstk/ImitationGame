#pragma once
// ����� ����: GameObject �� ����ص� ��� ���� �� ����. 
class GameActor : public GameObject
{
public:
	GameActor() {};
	~GameActor(){};
	// Command ���
	virtual void LeftMove() = 0;
	virtual void RightMove() = 0;
	virtual void Jump() = 0;
};
