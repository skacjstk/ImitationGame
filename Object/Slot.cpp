#include "ImitationGame/framework.h"
#include "./Object/Item.h"
#include "./Object/Button.h"
#include "Slot.h"

Slot::Slot(SlotType type)
{
	SetActive(false);
	this->SetSlotType(type);

	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	wstring strImage = IMAGE_FOLDER;
	wstring strHoverImage = IMAGE_FOLDER;	
	switch (type)
	{
		case SlotType::WEAPON:
			strImage += L"Inventory/IconWeapon.png";
			strHoverImage += L"Inventory/IconWhite.png";
			break;
		case SlotType::SUBWEAPON:
			strImage += L"Inventory/IconSubWeapon.png";
			strHoverImage += L"Inventory/IconWhite.png";
			break;
		case SlotType::ACCESSORY:
			strImage += L"Inventory/IconRing.png"; 
			strHoverImage += L"Inventory/IconWhite #237541.png";
			break;
		default:
			strImage += L"Inventory/IconGray.png"; 
			strHoverImage += L"Inventory/IconWhite #237541.png";
			break;
	}
	// Next:: 호버링 이미지도 타입별로 바꿔질 예정
	
	slot_ = new Button(strImage, strHoverImage, strShader);
	slot_->SetButtonName(L"");

	// 슬롯 아이템은 nullptr 맞긴 함.( inventory에서 데이터 읽어 포인팅할 예정 )
}

// Slot::Slot(Slot& slot)
// {
// 	printf("Slot 복사 생성자 호출\n");
// }
// 
// Slot::Slot(Slot&& slot) noexcept
// {
// 	printf("Slot 이동 생성자 호출\n");
// }

Slot::~Slot()
{
	SAFE_DELETE(slot_);
	SAFE_DELETE(slotItem_);
}

void Slot::Update(Matrix V, Matrix P)
{
	slot_->SetScale(this->GetScale());
	slot_->SetPosition(this->GetPosition());
	slot_->Update(V, P);

	if (slotItem_ != nullptr) {
		slotItem_->SetScale(this->GetScale());
		slotItem_->SetPosition(this->GetPosition());
		slotItem_->FrontImageUpdate(V, P);
	}
}

void Slot::Render()
{
	slot_->Render();
	if (slotItem_ != nullptr)
		slotItem_->FrontImageRender();
}

void Slot::Reset()
{
	SetActive(true);
}
