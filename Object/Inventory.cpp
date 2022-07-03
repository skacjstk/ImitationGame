#include "ImitationGame/framework.h"
#include "Object/Slot.h"
#include "./Object/Weapons/MeleeWeapons/ShortSword.h"
#include "Inventory.h"

Inventory::Inventory()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER;	strImage += L"Inventory/InventoryBase_2.png";
	wstring strShader = SHADER_FOLDER;	strShader += L"Texture.hlsl";
	invenBaseImage_ = new Texture(strImage, strShader);

	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i] = new Slot();
	}
	for (int i = 0; i < _countof(equipSlots_); ++i) {
	}
	// 인벤토리, Slot 크기위치 초기화

	// 인벤토리 크기위치 초기화
	SetScale(5.74468f * WSCALEX, 5.74468f * WSCALEY);
	invenBaseImage_->SetScale(this->GetScale());

	SetPosition(MAIN->GetWidth() * 0.5f - (int)invenBaseImage_->GetTextureRealSize().x * 0.5f, 0.0f);
	invenBaseImage_->SetPosition(this->GetPosition());

	// equipSlot 위치크기 초기화
	// equipSlots_ 는 01은 1번칸, 23은 2번칸 4567은 악세칸으로 고정할 예정.
	// Slot 위치크기 초기화
	float numX = 19 * 6.0f * WSCALEX;
	float numY = 19 * 6.0f * WSCALEY;

	// 장비칸 초기화
	for (int i = 0; i < _countof(equipSlots_) * 0.5; ++i) {
		if (i % 2 == 0){
			equipSlots_[i] = new Slot(Slot::SlotType::WEAPON);
		}
		else {
			equipSlots_[i] = new Slot(Slot::SlotType::SUBWEAPON);
		}
		equipSlots_[i]->SetScale(5.7f * WSCALEX, 5.7f * WSCALEY);
	}

	equipSlots_[0]->SetPosition(400.0f * WSCALEX, 284.0f * WSCALEY);
	equipSlots_[1]->SetPosition(numX + slotgap_ * WSCALEX + 2.0f * WSCALEX + 400.0f * WSCALEX, 284.0f * WSCALEY);
	equipSlots_[2]->SetPosition(710.0f * WSCALEX, 284.0f * WSCALEY);
	equipSlots_[3]->SetPosition(numX + slotgap_ * WSCALEX + 2.0f * WSCALEX + 710.0f * WSCALEX, 284.0f * WSCALEY);

	for (int i = 4; i < _countof(equipSlots_); ++i) {
		equipSlots_[i] = new Slot(Slot::SlotType::ACCESSORY);
		equipSlots_[i]->SetScale(5.7f * WSCALEX, 5.7f * WSCALEY);
		equipSlots_[i]->SetPosition((slotgap_ * (i-4)) + numX * i , 125.0f * WSCALEY);
	}

	//일반 아이템슬롯 위치 초기화
	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i] = new Slot();	// 디폴트 매개변수 OTHER
		itemSlots_[i]->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
		itemSlots_[i]->SetPosition((375.0f * WSCALEX) + (slotgap_ * (i % slotX_)) + (numX * (i % slotX_)),
			(-50.0f * WSCALEY) - (slotgap_ * (i / slotX_)) - numY * (i / slotX_));
	}
	// (-295.0f * WSCALEY) + (slotgap_ * (i / slotX_)) + numY * (i / slotX_)
	// 테스트코드
	itemSlots_[0]->SetItem(new ShortSword());
}

Inventory::~Inventory()
{
	SAFE_DELETE(invenBaseImage_);
}

void Inventory::Update(Matrix V, Matrix P)
{
}

void Inventory::Update(Matrix P)
{
	if (!IsActive())
		return;
	Matrix V = CAMERA->GetAbsoluteViewMatrix();
	invenBaseImage_->SetScale(this->GetScale());
	invenBaseImage_->Update(V, P);
	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i]->Update(V, P);
	}
	for (int i = 0; i < _countof(equipSlots_); ++i) {
		equipSlots_[i]->Update(V, P);
	}
}

void Inventory::Render()
{
	if (!IsActive())
		return;
	invenBaseImage_->Render();
	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i]->Render();
	}
	for (int i = 0; i < _countof(equipSlots_); ++i) {
		equipSlots_[i]->Render();
	}
}

void Inventory::Reset()
{

}
