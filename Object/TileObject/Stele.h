#pragma once
#include "Base/GameActor.h"
class Stele : public GameActor
{
public:		// ���� �ν��Ͻ� ����
	enum SteleState {
		OPEN = 0,
		OPENING = 1,
		CLOSING = 2,
		CLOSE = 3,

	};
	// �� Stele �� �������� �� Player�� ������ ����
	enum class StelePath
	{
		BOTTOM = 0,
		RIGHT = 1,
		TOP = 2,
		LEFT = 3
	};
	// ���׸��� ĳ���� �����Ҷ� ����� ��
	bool roomCleared_ = false;
private:	// ����� �ν��Ͻ� ����
	SteleState SteleState_ = SteleState::CLOSING;
	StelePath stelePath_ = StelePath::LEFT;
	class Player* ppPlayer = nullptr;
	std::function<void()> SwitchState;
	std::function<void()> Action;
	std::function<void()> Enter;
	std::array<class Texture*, 12> particles_ = {nullptr,};
	// �������϶� ����� ��
public:
	Stele();
	~Stele();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	
	void SetPath();	// �и� ������Ʈ �޾ƿͼ� Rotation�� �ʱ�ȭ ��. �� ���Ŀ� Rotation�� ������� Path ����
	Stele::StelePath GetPath() { return stelePath_; }
	bool CheckPlayer();

	// ���� ������ std::function���� ��ü ���ο� �����غ���
	// Open ����
private:
	void OpenSwitch();
	void OpenAction();
	void OpenEnter();
	// Open ����
	void OpeningSwitch();
	void OpeningAction();
	void OpeningEnter();
	// Closing ����
	void ClosingSwitch();
	void ClosingAction();
	void ClosingEnter();
	// Close ����
	void CloseSwitch();
	void CloseAction();
	void CloseEnter();
};