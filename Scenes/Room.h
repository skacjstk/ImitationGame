#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
class Room : public Scene
{
public:
	enum RoomType {
		DISABLE = 0,
		START = 1,
		END = 2,
		RESTAURANT = 4,
		SHOP = 8,
		NORMAL = 16
	};
	enum LinkedRoom {
		NONE = 0,	// �Ϲ������δ� ������ �ȵ�.
		TOP = 1,
		BOTTOM = 2,
		LEFT = 4,
		RIGHT = 8
	};
public:
	LinkedRoom myLinkedRoom_ = LinkedRoom::NONE;
	int myIndex[2] = { 0,0 };
	class Line* pLine = nullptr;
private:	//private instance variable
	int currentFloor_ = 0;
	RoomType roomType_ = RoomType::DISABLE;
	// ������Ʈ ����
	Texture* terrainImage_ = nullptr;	// ���� ���� 
	bool isCleared_ = false;

public:	//�� ��
	Room() : roomType_(RoomType::DISABLE) {};	// �ʱ�ȭ ����Ʈ
	Room(RoomType type) : roomType_(type) {};
	~Room();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;
	bool InitializeRoom();	// ���� �ʱ�ȭ�Ѵ� ( ���н� false ��ȯ)
public:	//public instance method
	RoomType GetRoomType() { return roomType_; }
	void SetRoomType(RoomType type) { roomType_ = type; }
	class Line* GetLines() override { return pLine; }
};
