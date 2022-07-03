#pragma once
class Inventory : public GameActor
{
private:
	Texture* invenBaseImage_ = nullptr;
	// 이 안에 작은 imgui를 만드는게 좋으려나?
	class Slot* equipSlots_[8] = { nullptr, };
	class Slot* itemSlots_[15] = { nullptr, };	// 첫 
	float slotgap_ = 10.0f;
	int slotX_ = 5;	// 가로 슬롯의 갯수: 얘는 슬롯 배치할때 연산용으로 씀
	int slotY_ = 3;	// 세로 슬롯의 갯수

public: // 생성자 & 소멸자
	Inventory();
	~Inventory();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P);	// 사용하지 않음.
	void Update(Matrix P);
	void Render();
	void Reset();
};
