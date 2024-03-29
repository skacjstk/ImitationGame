#pragma once
#include "./Object/NPC.h"
class Player : public GameActor
{
public:		// 공개 인스턴스 변수
	enum State
	{
		IDLE = 0,
		RUN = 1,
		JUMP = 2,
		ATTACK = 4,
		DASH = 8,	// 0725 추가했지만 모션은 Jump를 씀
		EAT = 16,
		DIE	= 32
	};
	// 던그리드 캐릭터 변경할때 사용할 것
	enum objectType
	{
		EXPLORER = 0,
		SUBJECT67 = 4
	};
	// 데이터 저장용
	struct PlayerData
	{
		int charCode = 0;
		UINT dashCount = 2;
		UINT maxDashCount = 2;
		float baseSpeed = 500.0f;
		float baseJumpSpeed = 150.0f;
		float baseLongJumpSpeed = 40.0f;	// 추가 점프용량임
		float dashRechargeTime = 1.5f;	// 1.5초마다 dashCount 추가
	};
private:	// 비공개 인스턴스 변수
	class InputHandler* inputHandler_;
	class Inventory* Inventory_ = nullptr;
	class Weapon* handedWeapon_[4] = { nullptr, };
//	Animation* _animation = nullptr;		 // GameActor에 일원화
	State _currentState = State::IDLE;
	objectType _heroType = objectType::EXPLORER;	// 던그리드 캐릭터 변경할때 사용할 것
	Vector2 _movePosition = Vector2(0.0f, 0.0f);	// lerp 하게 움직힐 거야
	bool isCanlongJump_ = true;	// 롱점프를 할 수 있느냐?
	bool isLongJump_ = false;	// 롱점프를 했느냐?
	float longJumpCount_ = 0.0f;
	int _moveCount = 0;
	bool _moveAble = true;
	float dashCycleTime_ = 0.0f;
	int currentFocusHand_ = 0;	// 0, 1 
	Texture* hand_[2] = { nullptr, };	// 무기에 장착되는건 왼손( 0번 )
	struct PlayerData playerData_;
	// Dash 관련
	std::function<void()> dashCB;
	float dashRadian = 0.0f;		// 대쉬 라디안이 0보다 크면 setY 안함. 그 대가로, dashRadian은 dash가 끝나고 0보다 작은 값으로 초기화됨.
	bool dashStart_ = false;	// 방금 대쉬를 했다면 true 바뀜 (GroundCheck() 에서 false로 바뀜)
	float dashLifeCycle = 0.5f;
	Texture* dashEffect_[3] = { nullptr, };
	bool isDash = false;
	NPC* interactionTarget_ = nullptr;
public:
	Player(int AnimationID = 0);
	~Player();
public:
	void Update(Matrix V, Matrix P) override;
	void GroundCheck() override;
	void RotateToMouse();
	void InputUpdate();
	void HandUpdate(Matrix V, Matrix P);
	void Render() override;
	void Reset() override;
	void ChangeChar(objectType playerType = objectType::EXPLORER);
	void CollisionCheck();
//	void GravityUpdate();
	void CycleUpdate();
	void UpdateHandedWeapon() override;
	void Attacked(float damage) override;
	void Dash() override;
	void DashDo();
	void DashWaiting();
public:
	// Setter
	void SetHP(int hp) { actorData_.HP = hp; }
	void SetState(Player::State state) { _currentState = state; }
	void SetMoveAble(bool moveable) { _moveAble = moveable; }
	void SetHandedWeapon(Weapon* item, int index);
	void SetGroundCheck(bool value) { isGround_ = value; }
	void SetInteractionTarget(NPC* target) { interactionTarget_ = target; }
	// Getter
	int GetHP() { return actorData_.HP; }
	bool IsPlay() { return _animation->IsPlay(); }	
	NPC* GetInteractionTarget() {	return interactionTarget_;	}
//	auto GetHandedWeapon(int index);
	Weapon* GetHandedWeapon(int index);
	int GetFocusHand() { return currentFocusHand_; }
	bool IsGround() { return isGround_; }
	void IncreaseHP(int amount);
	void DecreaseHP(int amount);
public:	// 움직임 관련 Command 함수
	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Idle() override;
	void Attack() override;
	void InventoryToggle() override;
	void SwapHandFocus() override;
	// 상태 관련 Command
	void HPChange() override;	// 체력이 변화할 경우, 이 함수를 호출해 주세요
	void Die() override;	// HP가 0이 되면 사망.
private: 
	void DashRecharge(int amount);
	void DashAnimationUpdate();	// DashDo 에서 수행되는, dashLifeCycle_ 과 연계되는 함수

};