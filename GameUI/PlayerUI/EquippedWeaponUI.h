#pragma once
#include "GameUI/BaseUI.h"
class EquippedWeaponUI : public BaseUI
{
	Texture* imageBase_[2] = { nullptr, };	// �������� Render �ؾ� ��.
	Texture* equipFrontImage_ = nullptr;
public: // ������ & �Ҹ���
	EquippedWeaponUI();
	~EquippedWeaponUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	Vector2 GetTextureRealSize() { return imageBase_[0]->GetTextureRealSize(); }	// dashUI ��ġ ������

};
