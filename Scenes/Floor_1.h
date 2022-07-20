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
	void GenerateRoom();
	void SetSRER(int& SX, int& SY, int& EX, int& EY);
	void ConnectSRER(int& SX, int& SY, int& EX, int& EY);
	void ConnectRoom(int x, int y);
	void GenerateRoomData();	// ���� �����͸� �־��ִ� �۾�
	class Line* GetGroundLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetGroundLines(); }
	class Line* GetCeilingLines() override { return roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->GetCeilingLines(); }
};
