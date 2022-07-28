#pragma once
#include "Object/NPC.h"
class Door : public NPC
{
public:		// ���� �ν��Ͻ� ����
	enum DoorState {
		OPEN = 0,
		CLOSING = 1,
		CLOSE = 2
	};
	// ���׸��� ĳ���� �����Ҷ� ����� ��
private:	// ����� �ν��Ͻ� ����
	DoorState doorState = DoorState::CLOSING;
	class Player* ppPlayer = { nullptr, };
	std::function<void()> SwitchState;
	std::function<void()> Enter;
	// �������϶� ����� ��
public:
	Door();
	~Door();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void Communicate() override;
	bool CheckPlayer();
	// ���� ������ std::function���� ��ü ���ο� �����غ���
	// Open ����
	void OpenSwitch();
	void OpenEnter();
	// Closing ����
	void ClosingSwitch();
	void ClosingEnter();
	// Close ����
	void CloseSwitch();
	void CloseEnter();
};