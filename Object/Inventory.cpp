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
	// �κ��丮, Slot ũ����ġ �ʱ�ȭ

	// �κ��丮 ũ����ġ �ʱ�ȭ
	SetScale(5.74468f * WSCALEX, 5.74468f * WSCALEY);
	invenBaseImage_->SetScale(this->GetScale());

	SetPosition(MAIN->GetWidth() * 0.5f - (int)invenBaseImage_->GetTextureRealSize().x * 0.5f, 0.0f);
	invenBaseImage_->SetPosition(this->GetPosition());

	// equipSlot ��ġũ�� �ʱ�ȭ
	// equipSlots_ �� 01�� 1��ĭ, 23�� 2��ĭ 4567�� �Ǽ�ĭ���� ������ ����.
	// Slot ��ġũ�� �ʱ�ȭ
	float numX = 19 * 6.0f * WSCALEX;
	float numY = 19 * 6.0f * WSCALEY;

	// ���ĭ �ʱ�ȭ
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

	//�Ϲ� �����۽��� ��ġ �ʱ�ȭ
	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i] = new Slot();	// ����Ʈ �Ű����� OTHER
		itemSlots_[i]->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
		itemSlots_[i]->SetPosition((375.0f * WSCALEX) + (slotgap_ * (i % slotX_)) + (numX * (i % slotX_)),
			(-50.0f * WSCALEY) - (slotgap_ * (i / slotX_)) - numY * (i / slotX_));
	}
	// (-295.0f * WSCALEY) + (slotgap_ * (i / slotX_)) + numY * (i / slotX_)
	// �׽�Ʈ�ڵ�
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
