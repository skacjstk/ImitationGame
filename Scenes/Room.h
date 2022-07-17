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
private:	//private instance variable
	int currentFloor_ = 0;
	int myIndex[2] = { 0,0 };
	RoomType roomType_ = RoomType::DISABLE;
	// 오브젝트 정보
	Texture* terrainImage_ = nullptr;	// 지형 정보 
	bool isCleared_ = false;

public:	//생 소
	Room();
	Room(RoomType type);
	~Room();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;

public:	//public instance method
	RoomType GetRoomType() { return roomType_; }
private:
	void OnButton(int id);
};
