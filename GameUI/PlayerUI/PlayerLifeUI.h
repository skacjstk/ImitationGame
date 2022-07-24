#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class PlayerLifeUI : public BaseUI
{
	class Texture* imageBase_ = nullptr;
	class Texture* imageBack_ = nullptr;
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
