#pragma once
class GameActor : public GameObject
{
protected:
	class Collider* pCollider_ = nullptr;
	class Animation* _animation = nullptr;
	float gravity_ = 0.0f;	// Actor들에게 적용되는 매 프레임마다 아래로 떨어지는 변화량
	bool isGround_ = false;	// Actor 자신이 땅에 닿았는지 확인할 변수
	bool isConflicted_ = false;	// 벽 제외 다른 곳에 부딧혔을 때, 현재는 땅과 동일 취급
	bool isFall = false;
	bool isJump = false;
	bool beforeGround_ = false;
public:
	GameActor() {};
	~GameActor(){};
	// Command 목록
	virtual void GroundCheck();
	void GravityUpdate();
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
