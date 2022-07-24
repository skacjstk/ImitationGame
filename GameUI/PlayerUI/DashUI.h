#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class DashUI : public BaseUI
{
	class Texture* left_ = nullptr;	// 2 x 8
	class Texture* right_ = nullptr;	// 2 x 8
	class Texture* base_ = nullptr;	// 9 x 8 
	class Texture* count_ = nullptr;	// 9 x 4
public: // ������ & �Ҹ���
	DashUI();
	~DashUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	Vector2  GetTextureRealSize() { return count_->GetTextureRealSize(); }

};
