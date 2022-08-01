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
	// Next: 이벤트 큐로 제어하기
	playerTemp = (Player*)OBJECTMANAGER->FindObject("player");
	UpdateItemImage();
	equipFrontImage_->SetScale(itemTemp->GetWeaponTransform().scale);
	
	SetActive(true);
}

void EquippedWeaponUI::SwapItem()
{
	if (currentFocusHand_ == 0)
		currentFocusHand_ = 1;
	else if (currentFocusHand_ == 1)
		currentFocusHand_ = 0;

	Audio->Play("SwapItem");
	UpdateItemImage();
}

void EquippedWeaponUI::UpdateItemImage()
{
	// 대표 이미지를 가져올 땐 index 0 번 또는 2번을 가져와야 함.
	itemTemp = playerTemp->GetHandedWeapon(currentFocusHand_ * 2);	// 오류 이유: OBJECTMANAGER 등록이 Reset보다 늦었음
	if (itemTemp == nullptr)
		equipFrontImage_ = nullptr;
	else
	{
		equipFrontImage_ = itemTemp->GetFrontImage();	// 버그 존재: 인벤토리 열면 사라짐.
	//	equipFrontImage_->SetPosition(imageBase_[0]->GetPosition()); // 있나없나 똑같다. 현재버그는 inven어쩌구
	}
}
