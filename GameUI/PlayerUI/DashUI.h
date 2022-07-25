#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class DashUI : public BaseUI
{
	class Texture* left_ = nullptr;	// 2 x 8
	class Texture* right_ = nullptr;	// 2 x 8
	class Texture* base_ = nullptr;	// 9 x 8 
	class Texture* count_ = nullptr;	// 9 x 4
	UINT currentDashCount = 2;
	UINT maxDashCount = 2;
	Vector2 rightPos = Vector2(0.0f, 0.0f);
	Vector2 baseGap = Vector2(0.0f, 0.0f);
	Vector2 firstBasePos = Vector2(0.0f, 0.0f);
public: // ������ & �Ҹ���
	DashUI();
	~DashUI();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	Vector2 GetTextureRealSize() { return count_->GetTextureRealSize(); }
	void InitializeDashCount(int maxDashCount);
	void Dash();
	void DashRender();	// Ư���� ��Ģ�� �ʿ���
public:
	void IncreaseDashCount(int amount);
	void DecreaseDashCount(int amount);

};
