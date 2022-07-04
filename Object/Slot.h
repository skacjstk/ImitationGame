#pragma once
class Slot : public GameObject
{
public:
	enum class SlotType
	{
		OTHER = 0,
		WEAPON = 1,
		SUBWEAPON = 2,
		ACCESSORY = 4
	};
private:
	enum SlotType slotType_ = SlotType::OTHER;
	class Button* slot_ = nullptr;
	class Item* slotItem_ = nullptr;
public: // ������ & �Ҹ���
	Slot(SlotType type = SlotType::OTHER);
//	Slot(Slot& slot);	// ���� ������
//	Slot(Slot&& slot) noexcept;// �̵� ������
	~Slot();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P);
	void Render();
	void Reset();
	// Setter
public:
	void SetSlotType(enum SlotType slot) { slotType_ = slot; }
	void SetItem(Item* pitem) { slotItem_ = pitem; }
	// Getter
public:
	SlotType GetSlotType() { return slotType_; }
	auto GetItem() { return slotItem_; }	// Item*
	auto GetSlotButton() { return slot_; }
};
