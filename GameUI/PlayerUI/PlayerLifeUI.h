#pragma once
#include "GameUI/BaseUI.h"
class PlayerLifeUI : public BaseUI
{
	Texture* imageBase_ = nullptr;
	Texture* imageBack_ = nullptr;
public: // 생성자 & 소멸자
	PlayerLifeUI();
	~PlayerLifeUI();
public:	// 공개 인스턴스 변수 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
public:
	Vector2 GetTextureRealSize() { return imageBase_->GetTextureRealSize(); }	// dashUI 위치 보정용

};
