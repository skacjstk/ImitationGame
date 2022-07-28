#pragma once
#include "Object/NPC.h"
class Door : public NPC
{
public:		// 공개 인스턴스 변수
	enum DoorState {
		OPEN = 0,
		CLOSING = 1,
		CLOSE = 2
	};
	// 던그리드 캐릭터 변경할때 사용할 것
private:	// 비공개 인스턴스 변수
	DoorState doorState = DoorState::CLOSING;
	class Player* ppPlayer = { nullptr, };
	std::function<void()> SwitchState;
	std::function<void()> Enter;
	// 상태패턴때 사용할 것
public:
	Door();
	~Door();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void Communicate() override;
	bool CheckPlayer();
	// 상태 패턴을 std::function으로 객체 내부에 구현해보기
	// Open 상태
	void OpenSwitch();
	void OpenEnter();
	// Closing 상태
	void ClosingSwitch();
	void ClosingEnter();
	// Close 상태
	void CloseSwitch();
	void CloseEnter();
};