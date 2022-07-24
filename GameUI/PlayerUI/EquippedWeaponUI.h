#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class EquippedWeaponUI : public BaseUI
{
private:
	int currentFocusHand_ = 0;
	class Texture* imageBase_[2] = { nullptr, };	// 역순으로 Render 해야 함.
	class Texture* equipFrontImage_ = nullptr;
	class Player* playerTemp;
	class Weapon* itemTemp;
public: // 생성자 & 소멸자
	EquippedWeaponUI();
	~EquippedWeaponUI();
public:	// 공개 인스턴스 변수 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	Vector2 GetTextureRealSize() { return imageBase_[0]->GetTextureRealSize(); }	// dashUI 위치 보정용
	void SwapItem();
	void UpdateItemImage();

};
