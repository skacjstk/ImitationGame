#pragma once
class SkelDog : public GameActor
{
public:
	class SkelDogState* currentState_ = nullptr;
	class SkelDogState* state_[2] = { nullptr, };
private:
	Animation* _animation = nullptr;
public: // 생성자 & 소멸자
	SkelDog();
	~SkelDog();
public:	// 공개 인스턴스 변수 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	void StateUpdate();

	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Move() override;		// 파생용: 0630 기준 Left,Right에서 정해준 방향으로 Move를 호출하는 역할. 
	void Idle() override;		// IDLE과 Attack은 꼭 있어야 함.
	void Attack() override;

};
