#pragma once

#include "Scenes/Room.h"
/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
const int maxX = 8;
const int maxY = 4;
class Floor_1 : public Scene
{
public:
	int currentFloor_ = 1;	// 여긴 1층이니까
	Vector2 startPos = Vector2(0.0f, 0.0f);	// 플레이어가 층 진입했을때의 위치
private:	//private instance variable
	Texture* backGround_ = nullptr;

	class Room* roomData_[maxX][maxY] = { nullptr, };
	int tempRoom[maxX][maxY] = {0,};	// 룸을 확정짓기 전 데이터형을 저장하는 곳
	int tempLinkeRoom[maxX][maxY] = { 0, };
	bool visitedRoom[maxX][maxY] = { false, };	// DFS 에서 사용할 방문여부 검사
	int currentActiveRoom_[2] = { 0,0 };

public:	//생 소
	Floor_1();
	~Floor_1();

public:	//override
	void Update() override;	// 상속받은 클래스에서 구현하라는 뜻
	void Render() override;	// 순수 가상함수 : java의 인터페이스와 비슷한데 
	void ChangeScene() override; // Scene 변경시 변경할 대상 Scene에 이 함수가 호출됨
	void ExitScene() override;	// 씬 나올 때 따로 설정해야 할 함수
	void MoveRoom(int x, int y) override;	// 이동해야할 index를 갔다줘
	Room* GetCurrentRoom() { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]; }
private:
	// 딱 한가지로 고정된 맵 형태
	void ReadStartRoomMinimal();		//테스트: 나중에는 룸 별로 minimal로 값을 가져와야 함(offset)
	void ApplyStartRoom();	// 0 0에 start room
	void ApplyEndRoom();	// 5 1에 EndRoom
	void ApplyOtherRoom();	// 10 20 21 31 41 에 그냥 잡몹 룸 ( NPC식당같은거 일단 빼고 )
	void ReadRoomData(int x, int y);	// Start, end는 여기서 구분할 게 아님.
		void ReadLines(Room* tempRoom, string coord);	// 3개 형태의 Line들 가져와 Room 에 적용하기
//		void ReadObjects(Room* tempRoom, int& coord);	// 오브젝트 desc 가져와 Room에 new 하기	// Room에 기능 옮김.
	
	
	// 절차적 지형생성 부분: 잠정폐기
//	void GenerateRoom();
//	void SetSRER(int& SX, int& SY, int& EX, int& EY);
//	void ConnectSRER(int& SX, int& SY, int& EX, int& EY);
	void ConnectRoom(int x, int y);
	void GenerateRoomData();	// 룸의 데이터를 넣어주는 작업
	class Line* GetGroundLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetGroundLines(); }
	class Line* GetCeilingLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetCeilingLines(); }
	class Line* GetPlatformLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetPlatformLines(); }
private:	// 던전 룸 관련
	void EnterRoom(/*좌표*/);	// 지금은 00 

};
