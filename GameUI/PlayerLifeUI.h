#pragma once
#include "GameUI/BaseUI.h"
class PlayerLifeUI : public BaseUI
{
public: // ������ & �Ҹ���
	PlayerLifeUI();
	~PlayerLifeUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;

};
