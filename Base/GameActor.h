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
	virtual void EquipChange(int index) {};	// �������� ��ü
	virtual void ItemDragAndDrop() {};	// ������ �巡�׾� ��� ( ���ü�� ���� )
	virtual void SwapHandFocus() {};
	virtual void UpdateHandedWeapon() {};	// �κ��丮���� ������ ���� ���ſ�
public:
	Collider* GetCollider() { return pCollider_; }
};
