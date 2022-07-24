#pragma once
#include "GameUI/BaseUI.h"
class EquippedWeaponUI : public BaseUI
{
	Texture* imageBase_[2] = { nullptr, };	// 역순으로 Render 해야 함.
	Texture* equipFrontImage_ = nullptr;
public: // 생성자 & 소멸자
	EquippedWeaponUI();
	~EquippedWeaponUI();
public:	// 공개 인스턴스 변수 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	Vector2 GetTextureRealSize() { return imageBase_[0]->GetTextureRealSize(); }	// dashUI 위치 보정용

};
