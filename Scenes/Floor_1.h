#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
class Floor_1 : public Scene, Room
{
private:	//private instance variable
	Texture* backGround_ = nullptr;

	class Room* roomData_[8][4] = { nullptr, };
	int tempRoom[8][4] = {0,};	// 룸을 확정짓기 전 데이터형을 저장하는 곳
	int currentActiveRoom_[2] = { 0,0 };

public:	//생 소
	Floor_1();
	~Floor_1();

public:	//override
	void Update() override;	// 상속받은 클래스에서 구현하라는 뜻
	void Render() override;	// 순수 가상함수 : java의 인터페이스와 비슷한데 
	void ChangeScene() override; // Scene 변경시 변경할 대상 Scene에 이 함수가 호출됨
	void ExitScene() override;	// 씬 나올 때 따로 설정해야 할 함수
private:
	void GenerateRoom();
	void SetSRER(int& SX, int& SY, int& EX, int& EY);
	void ConnectSRER(int& SX, int& SY, int& EX, int& EY);
};
