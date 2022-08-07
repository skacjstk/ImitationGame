#pragma once

#include "Scenes/Room.h"
/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
namespace F2{
const int maxX = 3;
const int maxY = 1;
}
class Floor_2 : public Scene
{
public:
	int currentFloor_ = 2;
	Vector2 startPos = Vector2(0.0f, 0.0f);	// �÷��̾ �� ������������ ��ġ
private:	//private instance variable
	Texture* backGround_ = nullptr;

	class Room* roomData_[F2::maxX][F2::maxY] = { nullptr, };
	int tempRoom[F2::maxX][F2::maxY] = {0,};	// ���� Ȯ������ �� ���������� �����ϴ� ��
	int tempLinkeRoom[F2::maxX][F2::maxY] = { 0, };
	bool visitedRoom[F2::maxX][F2::maxY] = { false, };	// DFS ���� ����� �湮���� �˻�
	int currentActiveRoom_[2] = { 0,0 };

public:	//�� ��
	Floor_2();
	~Floor_2();

public:	//override
	void Update() override;	// ��ӹ��� Ŭ�������� �����϶�� ��
	void Render() override;	// ���� �����Լ� : java�� �������̽��� ����ѵ� 
	void ChangeScene() override; // Scene ����� ������ ��� Scene�� �� �Լ��� ȣ���
	void ExitScene() override;	// �� ���� �� ���� �����ؾ� �� �Լ�
	void MoveRoom(int x, int y) override;	// �̵��ؾ��� index�� ������
	Room* GetCurrentRoom() { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]; }
private:
	// �� �Ѱ����� ������ �� ����
	void ReadStartRoomMinimal();		//�׽�Ʈ: ���߿��� �� ���� minimal�� ���� �����;� ��(offset)
	void ApplyStartRoom();	// 0 0�� start room
	void ApplyEndRoom();	// 5 1�� EndRoom
	void ApplyOtherRoom();	// 10 20 21 31 41 �� �׳� ��� �� ( NPC�Ĵ簰���� �ϴ� ���� )
	void ReadRoomData(int x, int y);	// Start, end�� ���⼭ ������ �� �ƴ�.
		void ReadLines(Room* tempRoom, string coord);	// 3�� ������ Line�� ������ Room �� �����ϱ�
//		void ReadObjects(Room* tempRoom, int& coord);	// ������Ʈ desc ������ Room�� new �ϱ�	// Room�� ��� �ű�.
		
	class Line* GetGroundLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetGroundLines(); }
	class Line* GetCeilingLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetCeilingLines(); }
	class Line* GetPlatformLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetPlatformLines(); }
private:	// ���� �� ����
	void EnterRoom();	// ������ currentActive ��ǥ�� �ٲ��, �װ� ������ Reset �ϰ� �ϸ�, CameraLock �� �ٲ�

};
