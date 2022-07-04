#pragma once
class Inventory : public GameActor
{
private:
	struct swapIndex {
		int slotType;	// 1�� equip, 2�� �׼�����, 3�� item
		int slotIndex;
	};
	Texture* invenBaseImage_ = nullptr;
	GameActor* inventoryOwner_ = nullptr;
	class InputHandler* inputHandler_ = nullptr;
	// �� �ȿ� ���� imgui�� ����°� ��������?
	class Slot* equipSlots_[8] = { nullptr, };
	class Slot* itemSlots_[15] = { nullptr, };	// ù 
	float slotgap_ = 10.0f;
	int slotX_ = 5;	// ���� ������ ����: ��� ���� ��ġ�Ҷ� ��������� ��
	int slotY_ = 3;	// ���� ������ ����
	struct swapIndex dragIndex_ = { 0,0 };
	struct swapIndex dropIndex_ = { 0,0 };
	class Slot** dragSlot_ = nullptr;
	class Slot** dropSlot_ = nullptr;
public: // ������ & �Ҹ���
	Inventory();
	~Inventory();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P) override;	// ������� ����.
	void Update(Matrix P);
	void Render() override;
	void Reset() override;
	void ChangeSlot(Slot** dragSlot, Slot** dropSlot);	// ������ �ٲ�ġ����.	
	// ChangeSlot���� ��� �����ۿ� ��ȭ�� ������ ���(�Ǽ�����) ��� �ٽ� ������Ʈ�ϴ� �Լ�
	void UpdateEquip(Slot* changedSlot, int slotIndex);		
	void SetInventoryOwner(GameActor* actor) { inventoryOwner_ = actor; }	// �÷��̾�� �κ��丮���� ������� ���� �۾�
private:
	void InputUpdate();
	void SetInvenTabGroup();
	void ItemDragAndDrop() override;
	void Drag();
	void Drop();
};
