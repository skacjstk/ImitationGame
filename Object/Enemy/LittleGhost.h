#pragma once
class LittleGhost : public GameActor
{
public:
	enum class LittleGhostStateEnum {
		MOVE,	// �÷��̾� �ֺ� ������
		ATTACK	// ��������� ���� �� ������ ����
	};
	LittleGhostStateEnum stateEnum_ = LittleGhostStateEnum::MOVE;	// �̳��� index ���. static_cast�� currentState_�� ������.
	class LittleGhostState* currentState_ = nullptr;
	class LittleGhostState* state_[2] = { nullptr, };

	class GameActor* chaseTarget_ = nullptr;	// �̰� ��� enemy�� ������ �ְ� �ϰ������... ������ ���̰ڳ� Next
	float moveSpeed = 300.0f;
	float jumpSpeed = 100.0f;
public: // ������ & �Ҹ���
	LittleGhost();
	~LittleGhost();
public:	// ���� �ν��Ͻ� ���� 
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
