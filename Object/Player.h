#pragma once
class Player : public GameActor
{
public:		// 공개 인스턴스 변수
	enum State
	{
		IDLE = 0,
		RUN,
		JUMP,
		ATTACK,
		DIE
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
		float baseSpeed = 300.0f;
		float baseJumpSpeed = 150.0f;
		float baseLongJumpSpeed = 40.0f;	// 추가 점프용량임
	};
private:	// 비공개 인스턴스 변수
	class InputHandler* inputHandler_;
	class Inventory* Inventory_ = nullptr;
	Animation* _animation = nullptr;
	State _currentState = State::IDLE;
	objectType _heroType = objectType::EXPLORER;	// 던그리드 캐릭터 변경할때 사용할 것
	Vector2 _movePosition = Vector2(0.0f, 0.0f);	// lerp 하게 움직힐 거야
	bool isCanlongJump_ = true;	// 롱점프를 할 수 있느냐?
	bool isLongJump_ = false;	// 롱점프를 했느냐?
	float longJumpCount_ = 0.0f;
	int _moveCount = 0;
	bool _moveAble = true;
	float _Time = 0.0f;
	int _HP = 0;
	int currentFocusHand_ = 0;	// 0, 1 
	class Weapon* handedWeapon_[4] = { nullptr, };

	float gravity_ = 0.0f;	// Actor들에게 적용되는 매 프레임마다 아래로 떨어지는 변화량
	bool isGround_ = false;	// Actor 자신이 땅에 닿았는지 확인할 변수
	struct PlayerData playerData_;
public:
	Player(int AnimationID = 0);
	~Player();
public:
	void Update(Matrix V, Matrix P) override;
	void InputUpdate();
	void Render() override;
	void Reset() override;
	void Reset(objectType playerType = objectType::EXPLORER);
	void GroundCheck();
	void GravityUpdate();
	void UpdateHandedWeapon() override;
public:
	// Setter
	void SetHP(int hp) { _HP = hp; }
	void SetState(Player::State state) { _currentState = state; }
	void SetMoveAble(bool moveable) { _moveAble = moveable; }
	void SetHandedWeapon(Weapon* item, int index);
	// Getter
	int GetHP() { return _HP; }
	bool IsPlay() { return _animation->IsPlay(); }	
	auto GetHandedWeapon(int index);
	int GetFocusHand() { return currentFocusHand_; }
public:	// 움직임 관련 Command 함수
	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Idle() override;
	void Attack() override;
	void InventoryToggle() override;
	void SwapHandFocus() override;
private: 
	void Move(Vector2& position);	// 해당 위치로 움직이려고 시도함

};