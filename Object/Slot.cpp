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
	// Next:: ȣ���� �̹����� Ÿ�Ժ��� �ٲ��� ����
	
	slot_ = new Button(strImage, strHoverImage, strShader);
	slot_->SetButtonName(L"");

	// ���� �������� nullptr �±� ��.( inventory���� ������ �о� �������� ���� )
}

// Slot::Slot(Slot& slot)
// {
// 	printf("Slot ���� ������ ȣ��\n");
// }
// 
// Slot::Slot(Slot&& slot) noexcept
// {
// 	printf("Slot �̵� ������ ȣ��\n");
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
