#pragma once
// ����� ����: GameObject �� ����ص� ��� ���� �� ����. 
class GameActor : public GameObject
{
protected:
	class Collider* pCollider_ = nullptr;
public:
	GameActor() {};
	~GameActor(){};
	// Command ���
	virtual void LeftMove() {};
	virtual void RightMove() {};
	virtual void Jump() {};
	virtual void Move() {};	// �Ļ���: 0630 ���� Left,Right���� ������ �������� Move�� ȣ���ϴ� ����. 
	virtual void Idle() {};		// IDLE�� Attack�� �� �־�� ��.
	virtual void Attack() {};
	virtual void InventoryToggle() {};
public:
	Collider* GetCollider() { return pCollider_; }
};
