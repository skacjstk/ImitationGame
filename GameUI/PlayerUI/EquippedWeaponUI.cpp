#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/Weapons/Weapon.h"
#include "GameUI/PlayerUI/PlayerUI.h"
#include "EquippedWeaponUI.h"

EquippedWeaponUI::EquippedWeaponUI()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"UI/EquippedWeaponBase.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	
	imageBase_[0] = new Texture(strImage, strShader);
	imageBase_[1] = new Texture(strImage, strShader);
}

EquippedWeaponUI::~EquippedWeaponUI()
{
	for (int i = 0; i < _countof(imageBase_); ++i)
		SAFE_DELETE(imageBase_[i]);
}

void EquippedWeaponUI::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;
	imageBase_[1]->Update(V, P);
	imageBase_[0]->Update(V, P);
	if (equipFrontImage_ != nullptr) {
		equipFrontImage_->SetPosition(imageBase_[0]->GetPosition());
		equipFrontImage_->Update(V, P);
	}
}

void EquippedWeaponUI::Render()
{
	if (IsActive() == false)
		return;
	imageBase_[1]->Render();
	imageBase_[0]->Render();
	if (equipFrontImage_ != nullptr) {
		equipFrontImage_->Render();
	}
}

void EquippedWeaponUI::Reset()
{
	imageBase_[0]->SetScale(GetScale());
	imageBase_[1]->SetScale(GetScale());
	imageBase_[0]->SetPosition(GetPosition());

	Vector2 temp = imageBase_[0]->GetTextureRealSize();


	imageBase_[1]->SetPosition((temp * 0.1f) + GetPosition());
	// Next: �̺�Ʈ ť�� �����ϱ�
	Player* playerTemp = (Player*)OBJECTMANAGER->FindObject("player");
	Weapon* itemTemp = playerTemp->GetHandedWeapon(0);	// ���� ����: OBJECTMANAGER ����� Reset���� �ʾ���
	equipFrontImage_ = itemTemp->GetFrontImage();	// ���� ����: �κ��丮 ���� �����.

	equipFrontImage_->SetScale(itemTemp->GetWeaponTransform().scale);
	
	SetActive(true);
}
