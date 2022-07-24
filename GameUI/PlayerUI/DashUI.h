#pragma once
#include "GameUI/BaseUI.h"
class DashUI : public BaseUI
{
	Texture* left_ = nullptr;	// 2 x 8
	Texture* right_ = nullptr;	// 2 x 8
	Texture* base_ = nullptr;	// 9 x 8 
	Texture* count_ = nullptr;	// 9 x 4
public: // ������ & �Ҹ���
	DashUI();
	~DashUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	Vector2  GetTextureRealSize() { return count_->GetTextureRealSize(); }

};
