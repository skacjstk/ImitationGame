#pragma once
class GameActor : public GameObject
{
public:
	enum class ActorType
	{
		Player,
		Enemy
	};
	struct ActorData	// 현재 HP와 무적타임만 가지고 있음. ( maxHP 등 나머지는 개별 객체가 가질 수 있음. )
	{
		int HP = 10;
		int maxHP = 10;
		int ImmuneTime = 0;
		ActorType type = ActorType::Enemy;
		float damage = 5.0f;	// 이건 몬스터용, 플레이어는 무기의 데미지를 인자가 따로 있다.
		float armor = 5.0f;	// 임시: 아머
	};
	struct ActorData actorData_;
	bool isGround_ = false;	// Actor 자신이 땅에 닿았는지 확인할 변수
	Vector2 moveAmount = Vector2(0.0f, 0.0f);	// Player 이동구조 변경. 이곳에 담아서 모든 충돌처리 후 한번에 보내는 방식으로
protected:
	class Collider* pCollider_ = nullptr;
	class Animation* _animation = nullptr;
	float gravity_ = 0.0f;	// Actor들에게 적용되는 매 프레임마다 아래로 떨어지는 변화량
	float beforeGravity_ = 0.0f;
	bool isConflicted_ = false;	// 다른 곳에 부딧혔을 때
	bool isPlatform_ = true;	// 바닥인데 platform 일 경우
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
	virtual void LeftMove() {};
	virtual void RightMove() {};
	virtual void Attacked(float damage);
	virtual void FatalBlow();	// 치명적인 일격 판정, 이후 뭐가 없으면 사망
	virtual void Die();
	virtual void Jump() {};
	virtual void Move() {};	// 파생용: 0630 기준 Left,Right에서 정해준 방향으로 Move를 호출하는 역할. 
	virtual void Idle() {};		// IDLE과 Attack은 꼭 있어야 함.
	virtual void Attack() {};
	virtual void InventoryToggle() {};
	virtual void EquipChange(int index) {};	// 장비아이템 교체
	virtual void ItemDragAndDrop() {};	// 아이템 드래그앤 드롭 ( 장비교체의 조건 )
	virtual void SwapHandFocus() {};
	virtual void UpdateHandedWeapon() {};	// 인벤토리에서 아이템 정보 갱신용
	virtual void Dash() {};
	virtual void HPChange();	// 체력이 변화하게 되면 이 함수를 호출해 주세요. 체력바 갱신용
	// Getter
	int GetImmuneFrame() { return ImmuneFrame_; }
	// Setter
	void SetImmuneFrame(int frame) { ImmuneFrame_ = frame; }
public:
	class Collider* GetCollider() { return pCollider_; }
};
