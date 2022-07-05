#pragma once
// 현재는 더미: GameObject 로 사용해도 상관 없을 것 같음. 
class GameActor : public GameObject
{
protected:
	class Collider* pCollider_ = nullptr;
public:
	GameActor() {};
	~GameActor(){};
	// Command 목록
	virtual void LeftMove() {};
	virtual void RightMove() {};
	virtual void Jump() {};
	virtual void Move() {};	// 파생용: 0630 기준 Left,Right에서 정해준 방향으로 Move를 호출하는 역할. 
	virtual void Idle() {};		// IDLE과 Attack은 꼭 있어야 함.
	virtual void Attack() {};
	virtual void InventoryToggle() {};
	virtual void EquipChange(int index) {};	// 장비아이템 교체
	virtual void ItemDragAndDrop() {};	// 아이템 드래그앤 드롭 ( 장비교체의 조건 )
	virtual void SwapHandFocus() {};
	virtual void UpdateHandedWeapon() {};	// 인벤토리에서 아이템 정보 갱신용
public:
	Collider* GetCollider() { return pCollider_; }
};
