#pragma once
#include "./Base/InputHandler.h"
class Player : public GameActor
{
public:		// ���� �ν��Ͻ� ����
	enum State
	{
		IDLE = 0,
		WALK,
		ATTACK,
		GOAL,
		SLATE
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
	};
private:	// ����� �ν��Ͻ� ����
	InputHandler inputHandler;
	Animation* _animation = nullptr;
	State _currentState = State::IDLE;
	objectType _heroType = objectType::EXPLORER;	// ���׸��� ĳ���� �����Ҷ� ����� ��
	Vector2 _movePosition = Vector2(0.0f, 0.0f);	// lerp �ϰ� ������ �ž�
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
public:
	// Setter
	void SetHP(int hp) { _HP = hp; }
	void SetState(Player::State state) { _currentState = state; }
	void SetMoveAble(bool moveable) { _moveAble = moveable; }
	// Getter
	int GetHP() { return _HP; }
	bool IsGoal() { if (_currentState == State::GOAL) return true; else return false; }
	bool IsPlay() { return _animation->IsPlay(); }	
public:	// ������ ���� Command �Լ�
	void LeftMove() override;
	void RightMove() override;
private: void Move(Vector2 & position);	// �ش� ��ġ�� �����̷��� �õ���(
public:
	void Jump() override;
	void Attack();

};