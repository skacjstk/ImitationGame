#pragma once
class GameActor : public GameObject
{
public:
	struct ActorData
	{
		int HP = 0;
		int ImmuneTime = 0;
	};
protected:
	struct ActorData actorData_;
	class Collider* pCollider_ = nullptr;
	class Animation* _animation = nullptr;
	float gravity_ = 0.0f;	// Actor들에게 적용되는 매 프레임마다 아래로 떨어지는 변화량
	bool isGround_ = false;	// Actor 자신이 땅에 닿았는지 확인할 변수
	bool isConflicted_ = false;	// 벽 제외 다른 곳에 부딧혔을 때
	bool isFall = false;
	bool isJump = false;
	bool beforeGround_ = false;
	// Actor의 무적시간을 정해줌 (player 제외 죄다 0, 이뮨프레임은 실질적인 소유자만 프레임마다 -1 하게됨)
	int ImmuneFrame_ = 0;	
	int maxImmuneFrame_ = 0;	// 초기화 시점 추천은Actor Reset. actorData.ImmuneTime * GetFrame
public:
	GameActor() {};
	~GameActor(){};
	// Command 목록
	virtual void GroundCheck();
	void GravityUpdate();
	virtual void Attacked();
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
	// Getter
	int GetImmuneFrame() { return ImmuneFrame_; }
	// Setter
	void SetImmuneFrame(int frame) { ImmuneFrame_ = frame; }
public:
	class Collider* GetCollider() { return pCollider_; }
};
