#pragma once
#include "./Base/InputHandler.h"
class Player : public GameActor
{
public:		// ���� �ν��Ͻ� ����
	enum State
	{
		IDLE = 0,
		RUN,
		JUMP,
		ATTACK,
		DIE
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
		float baseSpeed = 300.0f;
		float baseJumpSpeed = 150.0f;
		float baseLongJumpSpeed = 40.0f;	// �߰� �����뷮��
	};
private:	// ����� �ν��Ͻ� ����
	InputHandler inputHandler;
	Animation* _animation = nullptr;
	State _currentState = State::IDLE;
	objectType _heroType = objectType::EXPLORER;	// ���׸��� ĳ���� �����Ҷ� ����� ��
	Vector2 _movePosition = Vector2(0.0f, 0.0f);	// lerp �ϰ� ������ �ž�
	bool isCanlongJump_ = true;	// �������� �� �� �ִ���?
	bool isLongJump_ = false;	// �������� �ߴ���?
	float longJumpCount_ = 0.0f;
	int _moveCount = 0;
	bool _moveAble = true;
	float _Time = 0.0f;
	int _HP = 0;
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
public:
	// Setter
	void SetHP(int hp) { _HP = hp; }
	void SetState(Player::State state) { _currentState = state; }
	void SetMoveAble(bool moveable) { _moveAble = moveable; }
	// Getter
	int GetHP() { return _HP; }
	bool IsPlay() { return _animation->IsPlay(); }	
public:	// ������ ���� Command �Լ�
	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Idle() override;
	void Attack() override;
private: 
	void Move(Vector2& position);	// �ش� ��ġ�� �����̷��� �õ���

};