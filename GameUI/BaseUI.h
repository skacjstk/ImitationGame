#pragma once
#include "Base/GameObject.h"
class BaseUI : public GameObject
{
public: // ������ & �Ҹ���
	BaseUI();
	~BaseUI();
public:	// ���� �ν��Ͻ� ���� 
	virtual void Update(Matrix V, Matrix P) override {};
	virtual void Render()override {};
	virtual void Reset()override {};

};
