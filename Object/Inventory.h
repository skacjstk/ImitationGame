#pragma once
class Inventory : public GameActor
{
private:
	Texture* invenBaseImage_ = nullptr;
	// �� �ȿ� ���� imgui�� ����°� ��������?
	class Slot* equipSlots_[8] = { nullptr, };
	class Slot* itemSlots_[15] = { nullptr, };	// ù 
	float slotgap_ = 10.0f;
	int slotX_ = 5;	// ���� ������ ����: ��� ���� ��ġ�Ҷ� ��������� ��
	int slotY_ = 3;	// ���� ������ ����

public: // ������ & �Ҹ���
	Inventory();
	~Inventory();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P);	// ������� ����.
	void Update(Matrix P);
	void Render();
	void Reset();
};
