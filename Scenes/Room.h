#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
#include "Object/Line.h"
#include "Object/TileObject/Stele.h"
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
	map<wstring, GameObject*> roomObjects;
	LinkedRoom myLinkedRoom_ = LinkedRoom::NONE;
	RoomType roomType_ = RoomType::DISABLE;
	int myIndex[2] = { 0,0 };
	int currentFloor_ = 1;	// Floor �� ����ȭ��(��� �� ����)
	std::array<Vector2, 4> posOfDirection = {Vector2(0.0f,0.0f), };		// player �� n ����"����" ���� �� �־�� �� ��ġ  �����¿� �� ( ��<->��, ��<-> �� ���� �ѹ� ��ȯ�ؾ���)
	std::array<bool, 4> canDirection = {false, };		// �� �� �� ��  ������	�� �뿡�� ���� ���⿡�� �ü��ֳ�?
	class Line* GroundLine_ = nullptr;
	class Line* CeilingLine_ = nullptr;
	class Line* PlatformLine_ = nullptr;
	Texture* terrainImage_ = nullptr;	// ���� ���� 
private:	//private instance variable
	// ������Ʈ ����
	bool isCleared_ = false;
	int currentMonsterNumber_ = 0;
public:	//�� ��
	Room();	// �ʱ�ȭ ����Ʈ
	Room(RoomType type);
	Room(RoomType type, int x, int y, int currentFloor_);
	~Room();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;
	bool InitializeRoom();	// ���� �ʱ�ȭ�Ѵ� ( ���н� false ��ȯ)
	void LoadObjFile(string strFileName);
	void SetTerrainImage(wstring strFileName, Vector2 pos);
	GameObject* FindObject(wstring objName);
	void Reset();
	void DecreaseCurrentMonsterNumber();
public:	//public instance method
	RoomType GetRoomType() { return roomType_; }
	void SetRoomType(RoomType type) { roomType_ = type; }
	class Line* GetGroundLines() override { return GroundLine_; }
	class Line* GetCeilingLines() override { return CeilingLine_; }
	class Line* GetPlatformLines() override { return PlatformLine_; }
	// Getter
	void GetRoomObjectData();
	Vector2 GetPosOfDirection(int index) { return posOfDirection[index]; }
	bool IsCleared() { return isCleared_; }
private:
	void PushDirection(Stele::StelePath ePath, Vector2 position);	// ��� �� ��� �߰���.
	void RoomClear();
};
