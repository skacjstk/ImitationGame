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
		WAIT = 4	// 모션은 IDLE의 1번을 공유할 예정
	};
	SkelState stateEnum_ = SkelState::IDLE;	// 이놈은 index 대용. static_cast로 currentState_에 주입함.
	class BigWhiteSkelState* currentState_ = nullptr;
	class BigWhiteSkelState* state_[5] = { nullptr, };
//	Animation* _animation = nullptr;	// GameActor에 넣음.
	class GameActor* chaseTarget_ = nullptr;	// 이걸 모든 enemy가 가지고 있게 하고싶은데... 공통을 못뽑겠네 Next
	float moveSpeed = 300.0f;
	float jumpSpeed = 100.0f;
private:

public: // 생성자 & 소멸자
	BigWhiteSkel();
	~BigWhiteSkel();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void StateUpdate();
	void AttackCheck();

	void Jump() override;
	void Move() override;		// 파생용: 0630 기준 Left,Right에서 정해준 방향으로 Move를 호출하는 역할. 
	void Idle() override;		// IDLE과 Attack은 꼭 있어야 함.
	void Attack() override;

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
	Animation* GetAnimation() { return _animation; }
};
