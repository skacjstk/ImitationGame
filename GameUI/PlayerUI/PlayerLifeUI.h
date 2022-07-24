#pragma once
#include "GameUI/BaseUI.h"
class PlayerLifeUI : public BaseUI
{
	Texture* imageBase_ = nullptr;
	Texture* imageBack_ = nullptr;
public: // ������ & �Ҹ���
	PlayerLifeUI();
	~PlayerLifeUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
public:
	Vector2 GetTextureRealSize() { return imageBase_->GetTextureRealSize(); }	// dashUI ��ġ ������

};
