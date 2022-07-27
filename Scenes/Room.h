#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
#include "Object/Line.h"
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
	map<string, GameObject*> roomObjects;
	LinkedRoom myLinkedRoom_ = LinkedRoom::NONE;
	RoomType roomType_ = RoomType::DISABLE;
	int myIndex[2] = { 0,0 };
	int currentFloor_ = 0;	// Floor �� ����ȭ��(��� �� ����)
	class Line* GroundLine_ = nullptr;
	class Line* CeilingLine_ = nullptr;
	class Line* PlatformLine_ = nullptr;
	Texture* terrainImage_ = nullptr;	// ���� ���� 
private:	//private instance variable
	// ������Ʈ ����
	bool isCleared_ = false;

public:	//�� ��
	Room() : roomType_(RoomType::DISABLE) {};	// �ʱ�ȭ ����Ʈ
	Room(RoomType type) : roomType_(type) {};
	Room(RoomType type, int x, int y) : roomType_(type), myIndex{ x,y } {};
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
	class Line* GetGroundLines() override { return GroundLine_; }
	class Line* GetCeilingLines() override { return CeilingLine_; }
	class Line* GetPlatformLines() override { return PlatformLine_; }
	// Getter
	void GetRoomObjectData();
};
