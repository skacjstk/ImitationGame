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
public: // 생성자 & 소멸자
	DashUI();
	~DashUI();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	Vector2 GetTextureRealSize() { return count_->GetTextureRealSize(); }
	void InitializeDashCount(int maxDashCount);
	void Dash();
	void DashRender();	// 특수한 규칙이 필요함
public:
	void IncreaseDashCount(int amount);
	void DecreaseDashCount(int amount);

};
