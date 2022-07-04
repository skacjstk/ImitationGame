#pragma once
class Inventory : public GameActor
{
private:
	Texture* invenBaseImage_ = nullptr;
	GameActor* inventoryOwner_ = nullptr;
	class InputHandler* inputHandler_ = nullptr;
	// 이 안에 작은 imgui를 만드는게 좋으려나?
	class Slot* equipSlots_[8] = { nullptr, };
	class Slot* itemSlots_[15] = { nullptr, };	// 첫 
	float slotgap_ = 10.0f;
	int slotX_ = 5;	// 가로 슬롯의 갯수: 얘는 슬롯 배치할때 연산용으로 씀
	int slotY_ = 3;	// 세로 슬롯의 갯수
	class Slot** dragSlot_ = nullptr;
	class Slot** dropSlot_ = nullptr;
public: // 생성자 & 소멸자
	Inventory();
	~Inventory();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P) override;	// 사용하지 않음.
	void Update(Matrix P);
	void Render() override;
	void Reset() override;
	void ChangeSlot(Slot** dragSlot, Slot** dropSlot);	// 슬롯을 바꿔치기함.	
	// ChangeSlot에서 장비 아이템에 변화가 생겼을 경우(악세포함) 장비를 다시 업데이트하는 함수
	void UpdateEquip(Slot** UpdateSlot);
	void SetInventoryOwner(GameActor* actor) { inventoryOwner_ = actor; }	// 플레이어와 인벤토리간의 연결고리를 위한 작업
private:
	void InputUpdate();
	void SetInvenTabGroup();
	void ItemDragAndDrop() override;
	void Drag();
	void Drop();
};
