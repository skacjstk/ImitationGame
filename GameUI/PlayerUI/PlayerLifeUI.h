#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class PlayerLifeUI : public BaseUI
{
	class Texture* imageBase_ = nullptr;
	class Texture* imageBack_ = nullptr;
	Texture* lifeBar_ = nullptr;
	Vector2 lifeBarBaseScale = Vector2(0.0f, 0.0f);	
	Vector2 modifyScale = Vector2(0.0f, 0.0f);	// x scale을 조정해 체력바를 표현할 것
	// 체력 바에 표시할 숫자
	wstring hpDesc = L"";
	Vector2 hpDescPos = Vector2(0.0f, 0.0f);
	float font_size = 45.0f;
public: // 생성자 & 소멸자
	PlayerLifeUI();
	~PlayerLifeUI();
public:	// 공개 인스턴스 변수 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
public:
	Vector2 GetTextureRealSize() { return imageBase_->GetTextureRealSize(); }	// dashUI 위치 보정용
	void  UpdateLifeBar(int currentHp, int maxHP);
};
