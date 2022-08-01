#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
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
		NONE = 0,	// 일반적으로는 가지면 안됨.
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
	int currentFloor_ = 1;	// Floor 와 동기화되(어야 할 변수)
	std::array<Vector2, 4> posOfDirection = {Vector2(0.0f,0.0f), };		// player 가 n 방향"으로" 왔을 때 있어야 할 위치  상하좌우 순 ( 좌<->우, 상<-> 하 끼리 한번 교환해야함)
	std::array<bool, 4> canDirection = {false, };		// 하 우 상 좌  순서로	이 룸에서 이쪽 방향에서 올수있냐?
	class Line* GroundLine_ = nullptr;
	class Line* CeilingLine_ = nullptr;
	class Line* PlatformLine_ = nullptr;
	Texture* terrainImage_ = nullptr;	// 지형 정보 
private:	//private instance variable
	// 오브젝트 정보
	bool isCleared_ = false;

public:	//생 소
	Room();	// 초기화 리스트
	Room(RoomType type);
	Room(RoomType type, int x, int y, int currentFloor_);
	~Room();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;
	bool InitializeRoom();	// 룸을 초기화한다 ( 실패시 false 반환)
	void LoadObjFile(string strFileName);
	void SetTerrainImage(wstring strFileName, Vector2 pos);
	GameObject* FindObject(wstring objName);
	void Reset();
public:	//public instance method
	RoomType GetRoomType() { return roomType_; }
	void SetRoomType(RoomType type) { roomType_ = type; }
	class Line* GetGroundLines() override { return GroundLine_; }
	class Line* GetCeilingLines() override { return CeilingLine_; }
	class Line* GetPlatformLines() override { return PlatformLine_; }
	// Getter
	void GetRoomObjectData();
	Vector2 GetPosOfDirection(int index) { return posOfDirection[index]; }
private:
	void PushDirection(Stele::StelePath ePath, Vector2 position);	// 헤더 안 헤더 추가함.
};
