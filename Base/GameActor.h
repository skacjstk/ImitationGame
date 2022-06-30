#pragma once
// 현재는 더미: GameObject 로 사용해도 상관 없을 것 같음. 
class GameActor : public GameObject
{
protected:
	float gravity_ = 0.0f;	// Actor들에게 적용되는 매 프레임마다 아래로 떨어지는 변화량
	bool isGround_ = false;	// Actor 자신이 땅에 닿았는지 확인할 변수
	class Collider* pCollider_ = nullptr;
public:
	GameActor() {};
	~GameActor(){};
	// Command 목록
	virtual void LeftMove() {};
	virtual void RightMove() {};
	virtual void Jump() {};
	virtual void Move() {};	// 파생용: 0630 기준 Left,Right에서 정해준 방향으로 Move를 호출하는 역할. 
	virtual void Idle() = 0;		// IDLE과 Attack은 꼭 있어야 함.
	virtual void Attack() = 0;
public:
	Collider* GetCollider() { return pCollider_; }
};
