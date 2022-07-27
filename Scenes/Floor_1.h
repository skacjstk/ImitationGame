#pragma once

#include "Scenes/Room.h"
/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
const int maxX = 8;
const int maxY = 4;
class Floor_1 : public Scene
{
public:
	int currentFloor_ = 1;	// ���� 1���̴ϱ�
private:	//private instance variable
	Texture* backGround_ = nullptr;

	class Room* roomData_[maxX][maxY] = { nullptr, };
	int tempRoom[maxX][maxY] = {0,};	// ���� Ȯ������ �� ���������� �����ϴ� ��
	int tempLinkeRoom[maxX][maxY] = { 0, };
	bool visitedRoom[maxX][maxY] = { false, };	// DFS ���� ����� �湮���� �˻�
	int currentActiveRoom_[2] = { 0,0 };

public:	//�� ��
	Floor_1();
	~Floor_1();

public:	//override
	void Update() override;	// ��ӹ��� Ŭ�������� �����϶�� ��
	void Render() override;	// ���� �����Լ� : java�� �������̽��� ����ѵ� 
	void ChangeScene() override; // Scene ����� ������ ��� Scene�� �� �Լ��� ȣ���
	void ExitScene() override;	// �� ���� �� ���� �����ؾ� �� �Լ�
private:
	// �� �Ѱ����� ������ �� ����
	void ApplyStartRoom();	// 0 0�� start room
	void ApplyEndRoom();	// 5 1�� EndRoom
	void ApplyOtherRoom();	// 10 20 21 31 41 �� �׳� ��� �� ( NPC�Ĵ簰���� �ϴ� ���� )
	void ReadRoomData(int x, int y);	// Start, end�� ���⼭ ������ �� �ƴ�.
		void ReadLines(Room* tempRoom, int& coord);	// 3�� ������ Line�� ������ Room �� �����ϱ�
		void ReadObjects(Room* tempRoom, int& coord);	// ������Ʈ desc ������ Room�� new �ϱ�
	// connectRoom�� ��Ȱ�� ������ ��
	
	// ������ �������� �κ�: �������
	void GenerateRoom();
	void SetSRER(int& SX, int& SY, int& EX, int& EY);
	void ConnectSRER(int& SX, int& SY, int& EX, int& EY);
	void ConnectRoom(int x, int y);
	void GenerateRoomData();	// ���� �����͸� �־��ִ� �۾�
	class Line* GetGroundLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetGroundLines(); }
	class Line* GetCeilingLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetCeilingLines(); }
	class Line* GetPlatformLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetPlatformLines(); }
};
