#pragma once
// 현재는 더미: GameObject 로 사용해도 상관 없을 것 같음. 
class GameActor : public GameObject
{
public:
	GameActor() {};
	~GameActor(){};
	// Command 목록
	virtual void LeftMove() = 0;
	virtual void RightMove() = 0;
	virtual void Jump() = 0;
};
