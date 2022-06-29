#pragma once
// ����� ����: GameObject �� ����ص� ��� ���� �� ����. 
class GameActor : public GameObject
{
protected:
	float gravity_ = 0.0f;	// Actor�鿡�� ����Ǵ� �� �����Ӹ��� �Ʒ��� �������� ��ȭ��
	bool isGround_ = false;	// Actor �ڽ��� ���� ��Ҵ��� Ȯ���� ����
public:
	GameActor() {};
	~GameActor(){};
	// Command ���
	virtual void LeftMove() = 0;
	virtual void RightMove() = 0;
	virtual void Jump() = 0;
	virtual void Idle() = 0;
};
