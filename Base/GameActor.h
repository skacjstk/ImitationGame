#pragma once
// 현재는 더미: GameObject 로 사용해도 상관 없을 것 같음. 
class GameActor : public GameObject
{
protected:
	float gravity_ = 0.0f;	// Actor들에게 적용되는 매 프레임마다 아래로 떨어지는 변화량
	bool isGround_ = false;	// Actor 자신이 땅에 닿았는지 확인할 변수
public:
	GameActor() {};
	~GameActor(){};
	// Command 목록
	virtual void LeftMove() = 0;
	virtual void RightMove() = 0;
	virtual void Jump() = 0;
	virtual void Idle() = 0;
};
