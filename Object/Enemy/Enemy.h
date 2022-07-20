#pragma once
class Enemy : public GameActor
{
public: // 생성자 & 소멸자
	Enemy();
	~Enemy();
public:	// 공개 인스턴스 변수 
	virtual void Update(Matrix V, Matrix P) = 0;  // 순수가상함수
	virtual void Render() = 0;                    // 순수가상함수
	virtual void Reset() = 0;                    // 순수가상함수
};
