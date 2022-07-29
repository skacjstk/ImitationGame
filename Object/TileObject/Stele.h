#pragma once
class Stele : public GameActor
{
public:		// 공개 인스턴스 변수
	enum SteleState {
		OPEN = 0,
		OPENING = 1,
		CLOSING = 2,
		CLOSE = 3,

	};
	// 이 Stele 에 도달했을 때 Player를 보내줄 방향
	enum class StelePath
	{
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};
	// 던그리드 캐릭터 변경할때 사용할 것
private:	// 비공개 인스턴스 변수
	SteleState SteleState_ = SteleState::CLOSING;
	StelePath stelePath_ = StelePath::LEFT;
	class Player* ppPlayer = nullptr;
	std::function<void()> SwitchState;
	std::function<void()> Action;
	std::function<void()> Enter;
	std::array<class Texture*, 12> particles_ = {nullptr,};
	// 상태패턴때 사용할 것
public:
	Stele();
	~Stele();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	
	void SetPath();	// 분명 오브젝트 받아와서 Rotation을 초기화 함. 그 이후에 Rotation을 기반으로 Path 설정
	bool CheckPlayer();
	// 상태 패턴을 std::function으로 객체 내부에 구현해보기
	// Open 상태
	void OpenSwitch();
	void OpenAction();
	void OpenEnter();
	// Open 상태
	void OpeningSwitch();
	void OpeningAction();
	void OpeningEnter();
	// Closing 상태
	void ClosingSwitch();
	void ClosingAction();
	void ClosingEnter();
	// Close 상태
	void CloseSwitch();
	void CloseAction();
	void CloseEnter();
};