#pragma once
class LittleGhost : public GameActor
{
public:
	enum class LittleGhostStateEnum {
		MOVE,	// 플레이어 주변 떠돌기
		ATTACK	// 가까워지면 조금 더 빠르게 돌진
	};
	LittleGhostStateEnum stateEnum_ = LittleGhostStateEnum::MOVE;	// 이놈은 index 대용. static_cast로 currentState_에 주입함.
	class LittleGhostState* currentState_ = nullptr;
	class LittleGhostState* state_[2] = { nullptr, };

	class GameActor* chaseTarget_ = nullptr;	// 이걸 모든 enemy가 가지고 있게 하고싶은데... 공통을 못뽑겠네 Next
	float moveSpeed = 300.0f;
	float jumpSpeed = 100.0f;
public: // 생성자 & 소멸자
	LittleGhost();
	~LittleGhost();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void StateUpdate();
	void AttackCheck();

	void Move() override;
	void Move(float& radian);
	void Attack() override;
	void Attack(float& radian, float& dX, float& dY);
	void Dying() override;

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
	Animation* GetAnimation() { return _animation; }
};
