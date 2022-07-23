#pragma once
class BigWhiteSkel : public GameActor
{
public:
	enum class SkelState	
	{
		IDLE = 0,
		MOVE = 1,
		ATTACK = 2,
		JUMP = 3,
		WAIT = 4	// ����� IDLE�� 1���� ������ ����
	};
	SkelState stateEnum_ = SkelState::IDLE;	// �̳��� index ���. static_cast�� currentState_�� ������.
	class BigWhiteSkelState* currentState_ = nullptr;
	class BigWhiteSkelState* state_[5] = { nullptr, };
//	Animation* _animation = nullptr;	// GameActor�� ����.
	class GameActor* chaseTarget_ = nullptr;	// �̰� ��� enemy�� ������ �ְ� �ϰ�������... ������ ���̰ڳ� Next
	float moveSpeed = 300.0f;
	float jumpSpeed = 100.0f;
private:

public: // ������ & �Ҹ���
	BigWhiteSkel();
	~BigWhiteSkel();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void StateUpdate();
	void AttackCheck();

	void Jump() override;
	void Move() override;		// �Ļ���: 0630 ���� Left,Right���� ������ �������� Move�� ȣ���ϴ� ����. 
	void Idle() override;		// IDLE�� Attack�� �� �־�� ��.
	void Attack() override;
	void Attack(float& dX);

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
	Animation* GetAnimation() { return _animation; }
};