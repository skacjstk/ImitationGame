#include "ImitationGame/framework.h"
#include "Object/Slot.h"
#include "./Command/Command.h"	// �ƴѰ�
#include "./Object/Weapons/MeleeWeapons/ShortSword.h"
#include "./InputHandler/InventoryInputHandler.h"
#include "./InputHandler/InputHandler.h"
#include "./Object/Player.h"
#include "Inventory.h"

Inventory::Inventory()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER;	strImage += L"Inventory/InventoryBase_2.png";
	wstring strShader = SHADER_FOLDER;	strShader += L"Texture.hlsl";
	invenBaseImage_ = new Texture(strImage, strShader);

	//focusHandImage ����
	strImage = IMAGE_FOLDER;	strImage += L"Inventory/focusHand.png";
	focusHandImage_ = new Texture(strImage, strShader);

	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i] = new Slot();
	}
	// �κ��丮, Slot ũ����ġ �ʱ�ȭ

	// �κ��丮 ũ����ġ �ʱ�ȭ
	SetScale(5.74468f * WSCALEX, 5.74468f * WSCALEY);
	invenBaseImage_->SetScale(this->GetScale());

	SetPosition(MAIN->GetWidth() * 0.5f - (int)invenBaseImage_->GetTextureRealSize().x * 0.5f, 0.0f);
	invenBaseImage_->SetPosition(this->GetPosition());
	focusHandImage_->SetPosition(this->GetPosition());
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
	focusHandImage_->SetScale(5.74468f * WSCALEX, 5.74468f * WSCALEY);
	SetInvenTabGroup();
	SetFocusPosition();
	// �׽�Ʈ�ڵ�
	equipSlots_[0]->SetItem(new ShortSword());
	itemSlots_[0]->SetItem(new ShortSword());
	inputHandler_ = (InputHandler*)new InventoryInputHandler();
}

Inventory::~Inventory()
{
	SAFE_DELETE(invenBaseImage_);
	for (int i = 0; i < _countof(itemSlots_); ++i)
		SAFE_DELETE(itemSlots_[i]);
	for (int i = 0; i < _countof(equipSlots_); ++i)
		SAFE_DELETE(equipSlots_[i]);
	SAFE_DELETE(inputHandler_);
	SAFE_DELETE(focusHandImage_);
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

	InputUpdate();
	FocusImageUpdate(V,P);
	
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
	focusHandImage_->Render();
}

void Inventory::Reset()
{
}
// ���� �� ��ü�� �Ͼ�� �� ������.
void Inventory::ChangeSlot(Slot** dragSlot, Slot** dropSlot)
{
	// Next: ������ Ÿ�Կ� ���� (weapon�� �۾��Ұ� �� ����) ���� �� ��ȯ �˻� �߰�
	Item* temp = (*dragSlot)->GetItem();
	(*dragSlot)->SetItem((*dropSlot)->GetItem());
	(*dropSlot)->SetItem(temp);
}
void Inventory::FocusImageUpdate(Matrix V, Matrix P)
{
	focusHandImage_->Update(V, P);
}
//void Inventory::UpdateEquip(){} �� �ǾƷ���

void Inventory::OpenInventory()
{
	dropSlot_ = nullptr;
	dragSlot_ = nullptr;
	Audio->Play("openInventory");
	SetActive(true);
}

void Inventory::CloseInventory()
{
	SetActive(false);
}

// ����� currentFocusHand_ �� ���� ������� Image ( �Ͼ� �ٰ��� ) �� ��ġ���� �ٲ���.
void Inventory::SetFocusPosition()
{
	if (currentFocusHand_ == 0)
		focusHandImage_->SetPosition(463.0f * WSCALEX, 302.0f * WSCALEY);
	else if (currentFocusHand_ == 1)
		focusHandImage_->SetPosition(773.0f * WSCALEX, 302.0f * WSCALEY);
}

Item * Inventory::GetEquipItem(int index)
{
	return equipSlots_[index]->GetItem();
}

void Inventory::InputUpdate()
{
	vector<Command*> commandQueue = (*inputHandler_).handleInput();	// ��¥�� ť�� �ƴϾ�
	for (Command* command : commandQueue) {
		GameActor* tempActor = this;
		command->execute(*tempActor);
	}
}

void Inventory::SetInvenTabGroup()
{
	int itemCount = _countof(itemSlots_);
	int equipCount = _countof(equipSlots_);
	// �� ����
	itemSlots_[itemCount - 1]->GetSlotButton()->SetRightTabGroup(equipSlots_[0]->GetSlotButton());
	equipSlots_[equipCount - 1]->GetSlotButton()->SetRightTabGroup(itemSlots_[0]->GetSlotButton());

	for (int i = 0; i < equipCount - 1; ++i) {
		equipSlots_[i]->GetSlotButton()->SetRightTabGroup(equipSlots_[i + 1]->GetSlotButton());
	}
	for (int i = 0; i < itemCount - 1; ++i) {
		itemSlots_[i]->GetSlotButton()->SetRightTabGroup(itemSlots_[i + 1]->GetSlotButton());
	}

	// �� ����
	equipSlots_[0]->GetSlotButton()->SetLeftTabGroup(itemSlots_[itemCount - 1]->GetSlotButton());
	itemSlots_[0]->GetSlotButton()->SetLeftTabGroup(equipSlots_[equipCount - 1]->GetSlotButton());

	for (int i = equipCount -1; i > 0; --i) {
		equipSlots_[i]->GetSlotButton()->SetLeftTabGroup(equipSlots_[i - 1]->GetSlotButton());
	}
	for (int i = itemCount - 1; i > 0; --i) {
		itemSlots_[i]->GetSlotButton()->SetLeftTabGroup(itemSlots_[i - 1]->GetSlotButton());
	}
	// ���� ����
	for (UINT i = 0; i < equipCount / 2; ++i) {
		equipSlots_[i]->GetSlotButton()->SetTopTabGroup(itemSlots_[_countof(itemSlots_) - 1 - (equipCount / 2) + i]->GetSlotButton());
		equipSlots_[i]->GetSlotButton()->SetBottomTabGroup(equipSlots_[i+4]->GetSlotButton());

		equipSlots_[i + 4]->GetSlotButton()->SetTopTabGroup(equipSlots_[i]->GetSlotButton());
		equipSlots_[i + 4]->GetSlotButton()->SetBottomTabGroup(itemSlots_[i]->GetSlotButton());

		itemSlots_[i]->GetSlotButton()->SetTopTabGroup(equipSlots_[i + 4]->GetSlotButton());
		itemSlots_[_countof(itemSlots_) - 1 - (equipCount / 2) + i]->GetSlotButton()
			->SetBottomTabGroup(equipSlots_[i]->GetSlotButton());
	}
	// ���ڶ��� �ϵ��ڵ�
	itemSlots_[slotX_ - 1]->GetSlotButton()->SetTopTabGroup(equipSlots_[_countof(equipSlots_) - 1]->GetSlotButton());
	itemSlots_[_countof(itemSlots_) - 1]->GetSlotButton()->SetBottomTabGroup(equipSlots_[_countof(equipSlots_)/2 - 1]->GetSlotButton());

	for (UINT i = 0; i < slotX_; ++i) {
		itemSlots_[i]->GetSlotButton()->SetBottomTabGroup(itemSlots_[i + slotX_]->GetSlotButton());
		itemSlots_[i + slotX_]->GetSlotButton()->SetBottomTabGroup(itemSlots_[i + slotX_ + slotX_]->GetSlotButton());

		itemSlots_[i + slotX_ + slotX_]->GetSlotButton()->SetTopTabGroup(itemSlots_[i + slotX_]->GetSlotButton());
		itemSlots_[i + slotX_]->GetSlotButton()->SetTopTabGroup(itemSlots_[i]->GetSlotButton());
	}
	itemSlots_[0]->GetSlotButton()->SetActivate(true);
}
// Ŭ���� �������� ������ �巡��, ������ ��� �� SlotChange ȣ��.
void Inventory::ItemDragAndDrop()
{
	if (dragSlot_ == nullptr)
		Drag();	
	else 
		Drop();
	// 	Audio->Play("pickUpItem"); �� Drag�� ��� �������� ��, Drop�� �׳� ȣ���ϰ� �Ǿ�����.
}
void Inventory::Drag()
{
	int itemCount = _countof(itemSlots_);
	int equipCount = _countof(equipSlots_);

	// �ϴ� Ȱ��ȭ�� ��ư�� ������ dragSlot�� ����.
	bool isSelectSuccess = false;
	for (int i = 0; i < itemCount; ++i) {
		if (itemSlots_[i]->GetSlotButton()->IsActivate()) {
			dragSlot_ = &itemSlots_[i];
			isSelectSuccess = true; 
			break;
		}
	}
	// ������ ���� ������ ���������� �̰� ����.
	if (isSelectSuccess == false) {
		for (int i = 0; i < equipCount; ++i) {
			if (equipSlots_[i]->GetSlotButton()->IsActivate()) {
				dragSlot_ = &equipSlots_[i];
				isSelectSuccess = true;
				break;
			}
		}
	}
	// �ⲯ ã�� ���Կ� �������� ������
	if (isSelectSuccess == true) {
		if ((*dragSlot_)->GetItem() == nullptr) {
			isSelectSuccess = false;	// ������ ��ã����� �ϰ� nullptr�� �ٲٱ�
			dragSlot_ = nullptr;
		}
		else 
			Audio->Play("pickUpItem");		
	}

	// ���� ������ �������ֱ�
}
void Inventory::Drop()
{
	// Next: Drop ������ ��ü�� ��ȿ���� �˻��ؾ���. ( ���� ����� �Ǽ������� ������� �� ���� )
	bool dropUpdate = false;
	bool dragUpdate = false;
	bool isCheck = false;
	
	int itemCount = _countof(itemSlots_);
	int equipCount = _countof(equipSlots_);

	for (int i = 0; i < itemCount; ++i) {
		if (itemSlots_[i]->GetSlotButton()->IsActivate()) {
			dropSlot_ = &itemSlots_[i];
			isCheck = true;
			break;
		}
	}
	// itemĭ���� Drop �������� dropSlot�� ã������ �̰��� ������.
	for (int i = 0; i < equipCount; ++i) {
		if (isCheck == true)
			break;
		if (equipSlots_[i]->GetSlotButton()->IsActivate()) {
			dropSlot_ = &equipSlots_[i];
			break;
		}
	}

	// ������ �־��� ��ġ�� ���ĭ�̶�� Ȱ��ȭ��.
	if ((*dragSlot_)->GetSlotType() != Slot::SlotType::OTHER)	// ���� �ϳ��� ���Ȱ� �����ѰŸ� UpdateEquip �ϱ�
		dragUpdate = true;
	if ((*dropSlot_)->GetSlotType() != Slot::SlotType::OTHER)
		dropUpdate = true;

	ChangeSlot(dragSlot_, dropSlot_);
	if (dragUpdate || dropUpdate)
		inventoryOwner_->UpdateHandedWeapon();

	dragSlot_ = dropSlot_ = nullptr;	// ��ü �������� 
	Audio->Play("pickUpItem");
}
