#pragma once
class SkelDog : public GameActor
{
public:
	enum class DogState	
	{
		IDLE = 0,
		RUN = 1,
		ATTACK = 2,
		JUMP = 3,
		WAIT = 4	// ����� IDLE�� 1���� ������ ����
	};
	DogState stateEnum_ = DogState::IDLE;	// �̳��� index ���. static_cast�� currentState_�� ������.
	class SkelDogState* currentState_ = nullptr;
	class SkelDogState* state_[5] = { nullptr, };
//	Animation* _animation = nullptr;	// GameActor�� ����.
	class GameActor* chaseTarget_ = nullptr;	// �̰� ��� enemy�� ������ �ְ� �ϰ������... ������ ���̰ڳ� Next
	float moveSpeed = 400.0f;
	float jumpSpeed = 100.0f;
private:

public: // ������ & �Ҹ���
	SkelDog();
	~SkelDog();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void StateUpdate();
	void CollisionCheck();
	void AttackCheck();

	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Move() override;		// �Ļ���: 0630 ���� Left,Right���� ������ �������� Move�� ȣ���ϴ� ����. 
	void Idle() override;		// IDLE�� Attack�� �� �־�� ��.
	void Attack() override;
	void Attack(float& dX);

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
};
