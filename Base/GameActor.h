#pragma once
// ����� ����: GameObject �� ����ص� ��� ���� �� ����. 
class GameActor : public GameObject
{
protected:
	float gravity_ = 0.0f;	// Actor�鿡�� ����Ǵ� �� �����Ӹ��� �Ʒ��� �������� ��ȭ��
	bool isGround_ = false;	// Actor �ڽ��� ���� ��Ҵ��� Ȯ���� ����
	class Collider* pCollider_ = nullptr;
public:
	GameActor() {};
	~GameActor(){};
	// Command ���
	virtual void LeftMove() {};
	virtual void RightMove() {};
	virtual void Jump() {};
	virtual void Move() {};	// �Ļ���: 0630 ���� Left,Right���� ������ �������� Move�� ȣ���ϴ� ����. 
	virtual void Idle() = 0;		// IDLE�� Attack�� �� �־�� ��.
	virtual void Attack() = 0;
public:
	Collider* GetCollider() { return pCollider_; }
};
