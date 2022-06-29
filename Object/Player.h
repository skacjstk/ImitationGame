#pragma once
#include "./Base/InputHandler.h"
class Player : public GameActor
{
public:		// 공개 인스턴스 변수
	enum State
	{
		IDLE = 0,
		WALK,
		ATTACK,
		GOAL,
		SLATE
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
	};
private:	// 비공개 인스턴스 변수
	InputHandler inputHandler;
	Animation* _animation = nullptr;
	State _currentState = State::IDLE;
	objectType _heroType = objectType::EXPLORER;	// 던그리드 캐릭터 변경할때 사용할 것
	Vector2 _movePosition = Vector2(0.0f, 0.0f);	// lerp 하게 움직힐 거야
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
public:	// 움직임 관련 Command 함수
	void LeftMove() override;
	void RightMove() override;
private: void Move(Vector2 & position);	// 해당 위치로 움직이려고 시도함(
public:
	void Jump() override;
	void Attack();

};