#pragma once
#include "Base/GameObject.h"
class BaseUI : public GameObject
{
public: // 생성자 & 소멸자
	BaseUI();
	~BaseUI();
public:	// 공개 인스턴스 변수 
	virtual void Update(Matrix V, Matrix P) override {};
	virtual void Render()override {};
	virtual void Reset()override {};

};
