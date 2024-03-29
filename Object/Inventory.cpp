#include "ImitationGame/framework.h"
#include "Object/Slot.h"
#include "./Command/Command.h"	// 아닌가
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

	//focusHandImage 생성
	strImage = IMAGE_FOLDER;	strImage += L"Inventory/focusHand.png";
	focusHandImage_ = new Texture(strImage, strShader);

	for (int i = 0; i < _countof(itemSlots_); ++i) {
		itemSlots_[i] = new Slot();
	}
	// 인벤토리, Slot 크기위치 초기화

	// 인벤토리 크기위치 초기화
	SetScale(5.74468f * WSCALEX, 5.74468f * WSCALEY);
	invenBaseImage_->SetScale(this->GetScale());

	SetPosition(MAIN->GetWidth() * 0.5f - (int)invenBaseImage_->GetTextureRealSize().x * 0.5f, 0.0f);
	invenBaseImage_->SetPosition(this->GetPosition());
	focusHandImage_->SetPosition(this->GetPosition());
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
	focusHandImage_->SetScale(5.74468f * WSCALEX, 5.74468f * WSCALEY);
	SetInvenTabGroup();
	SetFocusPosition();
	// 테스트코드
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
// 슬롯 간 교체가 일어났을 때 수행함.
void Inventory::ChangeSlot(Slot** dragSlot, Slot** dropSlot)
{
	// Next: 아이템 타입에 따른 (weapon에 작업할게 또 있음) 장착 및 교환 검사 추가
	Item* temp = (*dragSlot)->GetItem();
	(*dragSlot)->SetItem((*dropSlot)->GetItem());
	(*dropSlot)->SetItem(temp);
}
void Inventory::FocusImageUpdate(Matrix V, Matrix P)
{
	focusHandImage_->Update(V, P);
}
//void Inventory::UpdateEquip(){} 는 맨아래에

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

// 변경된 currentFocusHand_ 의 값을 기반으로 Image ( 하얀 다각형 ) 의 위치값을 바꿔줌.
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
	vector<Command*> commandQueue = (*inputHandler_).handleInput();	// 진짜로 큐는 아니야
	for (Command* command : commandQueue) {
		GameActor* tempActor = this;
		command->execute(*tempActor);
	}
}

void Inventory::SetInvenTabGroup()
{
	int itemCount = _countof(itemSlots_);
	int equipCount = _countof(equipSlots_);
	// 우 설정
	itemSlots_[itemCount - 1]->GetSlotButton()->SetRightTabGroup(equipSlots_[0]->GetSlotButton());
	equipSlots_[equipCount - 1]->GetSlotButton()->SetRightTabGroup(itemSlots_[0]->GetSlotButton());

	for (int i = 0; i < equipCount - 1; ++i) {
		equipSlots_[i]->GetSlotButton()->SetRightTabGroup(equipSlots_[i + 1]->GetSlotButton());
	}
	for (int i = 0; i < itemCount - 1; ++i) {
		itemSlots_[i]->GetSlotButton()->SetRightTabGroup(itemSlots_[i + 1]->GetSlotButton());
	}

	// 좌 설정
	equipSlots_[0]->GetSlotButton()->SetLeftTabGroup(itemSlots_[itemCount - 1]->GetSlotButton());
	itemSlots_[0]->GetSlotButton()->SetLeftTabGroup(equipSlots_[equipCount - 1]->GetSlotButton());

	for (int i = equipCount -1; i > 0; --i) {
		equipSlots_[i]->GetSlotButton()->SetLeftTabGroup(equipSlots_[i - 1]->GetSlotButton());
	}
	for (int i = itemCount - 1; i > 0; --i) {
		itemSlots_[i]->GetSlotButton()->SetLeftTabGroup(itemSlots_[i - 1]->GetSlotButton());
	}
	// 상하 설정
	for (UINT i = 0; i < equipCount / 2; ++i) {
		equipSlots_[i]->GetSlotButton()->SetTopTabGroup(itemSlots_[_countof(itemSlots_) - 1 - (equipCount / 2) + i]->GetSlotButton());
		equipSlots_[i]->GetSlotButton()->SetBottomTabGroup(equipSlots_[i+4]->GetSlotButton());

		equipSlots_[i + 4]->GetSlotButton()->SetTopTabGroup(equipSlots_[i]->GetSlotButton());
		equipSlots_[i + 4]->GetSlotButton()->SetBottomTabGroup(itemSlots_[i]->GetSlotButton());

		itemSlots_[i]->GetSlotButton()->SetTopTabGroup(equipSlots_[i + 4]->GetSlotButton());
		itemSlots_[_countof(itemSlots_) - 1 - (equipCount / 2) + i]->GetSlotButton()
			->SetBottomTabGroup(equipSlots_[i]->GetSlotButton());
	}
	// 끝자락은 하드코딩
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
// 클릭한 아이템이 없으면 드래그, 있으면 드롭 후 SlotChange 호출.
void Inventory::ItemDragAndDrop()
{
	if (dragSlot_ == nullptr)
		Drag();	
	else 
		Drop();
	// 	Audio->Play("pickUpItem"); 은 Drag의 경우 성공했을 때, Drop은 그냥 호출하게 되어있음.
}
void Inventory::Drag()
{
	int itemCount = _countof(itemSlots_);
	int equipCount = _countof(equipSlots_);

	// 일단 활성화된 버튼의 슬롯을 dragSlot에 넣음.
	bool isSelectSuccess = false;
	for (int i = 0; i < itemCount; ++i) {
		if (itemSlots_[i]->GetSlotButton()->IsActivate()) {
			dragSlot_ = &itemSlots_[i];
			isSelectSuccess = true; 
			break;
		}
	}
	// 위에서 슬롯 선택이 성공했으면 이건 안함.
	if (isSelectSuccess == false) {
		for (int i = 0; i < equipCount; ++i) {
			if (equipSlots_[i]->GetSlotButton()->IsActivate()) {
				dragSlot_ = &equipSlots_[i];
				isSelectSuccess = true;
				break;
			}
		}
	}
	// 기껏 찾은 슬롯에 아이템이 없으면
	if (isSelectSuccess == true) {
		if ((*dragSlot_)->GetItem() == nullptr) {
			isSelectSuccess = false;	// 아이템 못찾은취급 하고 nullptr로 바꾸기
			dragSlot_ = nullptr;
		}
		else 
			Audio->Play("pickUpItem");		
	}

	// 대충 뭔가를 지정해주기
}
void Inventory::Drop()
{
	// Next: Drop 시점에 교체가 유효한지 검사해야함. ( 각종 무기와 악세서리를 만들었을 때 하자 )
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
	// item칸에서 Drop 시점에서 dropSlot을 찾았으면 이곳은 생략함.
	for (int i = 0; i < equipCount; ++i) {
		if (isCheck == true)
			break;
		if (equipSlots_[i]->GetSlotButton()->IsActivate()) {
			dropSlot_ = &equipSlots_[i];
			break;
		}
	}

	// 기존에 있었던 위치가 장비칸이라면 활성화됨.
	if ((*dragSlot_)->GetSlotType() != Slot::SlotType::OTHER)	// 둘중 하나라도 장비된거 변경한거면 UpdateEquip 하기
		dragUpdate = true;
	if ((*dropSlot_)->GetSlotType() != Slot::SlotType::OTHER)
		dropUpdate = true;

	ChangeSlot(dragSlot_, dropSlot_);
	if (dragUpdate || dropUpdate)
		inventoryOwner_->UpdateHandedWeapon();

	dragSlot_ = dropSlot_ = nullptr;	// 교체 다했으니 
	Audio->Play("pickUpItem");
}
