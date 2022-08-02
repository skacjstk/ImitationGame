#pragma once
class Player : public GameActor
{
public:		// ���� �ν��Ͻ� ����
	enum State
	{
		IDLE = 0,
		RUN = 1,
		JUMP = 2,
		ATTACK = 4,
		DASH = 8,	// 0725 �߰������� ����� Jump�� ��
		DIE	= 16
	};
	// ���׸��� ĳ���� �����Ҷ� ����� ��
	enum objectType
	{
		EXPLORER = 0,
		SUBJECT67 = 4
	};
	// ������ �����
	struct PlayerData
	{
		int charCode = 0;
		UINT dashCount = 2;
		UINT maxDashCount = 2;
		float baseSpeed = 500.0f;
		float baseJumpSpeed = 150.0f;
		float baseLongJumpSpeed = 40.0f;	// �߰� �����뷮��
		float dashRechargeTime = 1.5f;	// 1.5�ʸ��� dashCount �߰�
	};
private:	// ����� �ν��Ͻ� ����
	class InputHandler* inputHandler_;
	class Inventory* Inventory_ = nullptr;
	class Weapon* handedWeapon_[4] = { nullptr, };
//	Animation* _animation = nullptr;		 // GameActor�� �Ͽ�ȭ
	State _currentState = State::IDLE;
	objectType _heroType = objectType::EXPLORER;	// ���׸��� ĳ���� �����Ҷ� ����� ��
	Vector2 _movePosition = Vector2(0.0f, 0.0f);	// lerp �ϰ� ������ �ž�
	bool isCanlongJump_ = true;	// �������� �� �� �ִ���?
	bool isLongJump_ = false;	// �������� �ߴ���?
	float longJumpCount_ = 0.0f;
	int _moveCount = 0;
	bool _moveAble = true;
	float dashCycleTime_ = 0.0f;
	int currentFocusHand_ = 0;	// 0, 1 
	Texture* hand_[2] = { nullptr, };	// ���⿡ �����Ǵ°� �޼�( 0�� )
	struct PlayerData playerData_;
	// Dash ����
	std::function<void()> dashCB;
	float dashRadian = 0.0f;		// �뽬 ������ 0���� ũ�� setY ����. �� �밡��, dashRadian�� dash�� ������ 0���� ���� ������ �ʱ�ȭ��.
	bool dashStart_ = false;	// ��� �뽬�� �ߴٸ� true �ٲ� (GroundCheck() ���� false�� �ٲ�)
	float dashLifeCycle = 0.5f;
	Texture* dashEffect_[3] = { nullptr, };
	bool isDash = false;
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
	// Getter
	int GetHP() { return actorData_.HP; }
	bool IsPlay() { return _animation->IsPlay(); }	
//	auto GetHandedWeapon(int index);
	Weapon* GetHandedWeapon(int index);
	int GetFocusHand() { return currentFocusHand_; }
	bool IsGround() { return isGround_; }
	void IncreaseHP(int amount);
	void DecreaseHP(int amount);
public:	// ������ ���� Command �Լ�
	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Idle() override;
	void Attack() override;
	void InventoryToggle() override;
	void SwapHandFocus() override;
	// ���� ���� Command
	void HPChange() override;	// ü���� ��ȭ�� ���, �� �Լ��� ȣ���� �ּ���
	void Die() override;	// HP�� 0�� �Ǹ� ���.
private: 
	void DashRecharge(int amount);
	void DashAnimationUpdate();	// DashDo ���� ����Ǵ�, dashLifeCycle_ �� ����Ǵ� �Լ�

};