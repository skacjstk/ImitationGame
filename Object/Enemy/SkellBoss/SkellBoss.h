#pragma once
class SkellBoss : public GameActor
{
public:
	// IDLE ATTACK DYING DIE
	enum class eSkellBossState	
	{
		HIDE = 0,
		APPEAR = 1,
		WAIT = 2,
		BULLET = 3,
		SWORD = 4,
		LASER = 5,
		DYING = 6,
		DEAD = 7
	};
	Animation* hand_[2] = { nullptr, };	// IDLE, ATTACk ( LASER ) 0번이 왼손
	eSkellBossState stateEnum_ = eSkellBossState::HIDE;	// 이놈은 index 대용. static_cast로 currentState_에 주입함.
	std::function<void()> SwitchState;
	std::function<void()> Enter;
	std::function<void(Matrix, Matrix)> Action;
	class GameActor* chaseTarget_ = nullptr;	// 이걸 모든 enemy가 가지고 있게 하고싶은데... 공통을 못뽑겠네 Next
	float appearAlpha = 0.0f;
	float waitCycle_ = 0.0f;
	Vector2 handGap = Vector2(0.0f,0.0f);
	bool attacked_ = false;	// 공격할 경우 true로.
private:

public: // 생성자 & 소멸자
	SkellBoss();
	~SkellBoss();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
private:	// 상태객체 대신 상태 std::function<void()>
	bool CheckPlayer();	// 거리 측정
	void SwitchStateHIDE();
	void EnterHIDE();
	void ActionHIDE(Matrix V, Matrix P);

	void SwitchStateAPPEAR();
	void EnterAPPEAR();
	void ActionAPPEAR(Matrix V, Matrix P);

	void SwitchStateWAIT();
	void EnterWAIT();
	void ActionWAIT(Matrix V, Matrix P);

	void SwitchStateBULLET();
	void SwitchStateSWORD();
	void SwitchStateLASER();
	void SwitchStateDYING();
	void SwitchStateDEAD();

	void ActionBULLET(Matrix V, Matrix P);
	void ActionSWORD(Matrix V, Matrix P);
	void ActionLASER(Matrix V, Matrix P);
	void ActionDYING(Matrix V, Matrix P);
	void ActionDEAD(Matrix V, Matrix P);

	void EnterBULLET();
	void EnterSWORD();
	void EnterLASER();
	void EnterDYING();
	void EnterDEAD();		
};
