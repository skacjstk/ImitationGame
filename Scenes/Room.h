#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
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
		NONE = 0,	// 일반적으로는 가지면 안됨.
		TOP = 1,
		BOTTOM = 2,
		LEFT = 4,
		RIGHT = 8
	};
public:
	LinkedRoom myLinkedRoom_ = LinkedRoom::NONE;
	int myIndex[2] = { 0,0 };
	class Line* GroundLine_ = nullptr;
	class Line* CeilingLine_ = nullptr;
private:	//private instance variable
	int currentFloor_ = 0;
	RoomType roomType_ = RoomType::DISABLE;
	// 오브젝트 정보
	Texture* terrainImage_ = nullptr;	// 지형 정보 
	bool isCleared_ = false;

public:	//생 소
	Room() : roomType_(RoomType::DISABLE) {};	// 초기화 리스트
	Room(RoomType type) : roomType_(type) {};
	~Room();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;
	bool InitializeRoom();	// 룸을 초기화한다 ( 실패시 false 반환)
public:	//public instance method
	RoomType GetRoomType() { return roomType_; }
	void SetRoomType(RoomType type) { roomType_ = type; }
	class Line* GetGroundLines() override { return GroundLine_; }
	class Line* GetCeilingLines() override { return CeilingLine_; }
};
