#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
class Floor_1 : public Scene, Room
{
private:	//private instance variable
	Texture* backGround_ = nullptr;

	class Room* roomData_[8][4] = { nullptr, };
	int tempRoom[8][4] = {0,};	// ���� Ȯ������ �� ���������� �����ϴ� ��
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
};
