#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class EquippedWeaponUI : public BaseUI
{
private:
	int currentFocusHand_ = 0;
	class Texture* imageBase_[2] = { nullptr, };	// �������� Render �ؾ� ��.
	class Texture* equipFrontImage_ = nullptr;
	class Player* playerTemp;
	class Weapon* itemTemp;
public: // ������ & �Ҹ���
	EquippedWeaponUI();
	~EquippedWeaponUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	Vector2 GetTextureRealSize() { return imageBase_[0]->GetTextureRealSize(); }	// dashUI ��ġ ������
	void SwapItem();
	void UpdateItemImage();

};
