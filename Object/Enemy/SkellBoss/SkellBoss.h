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
	eSkellBossState stateEnum_ = eSkellBossState::HIDE;	// 이놈은 index 대용. static_cast로 currentState_에 주입함.
	Animation* hand_[2] = { nullptr, };	// IDLE, ATTACK ( LASER ) 0번이 왼손
	Animation* back_ = nullptr;	// 총알 쏠때 뒷배경
	std::function<void()> SwitchState;
	std::function<void()> Enter;
	std::function<void(Matrix, Matrix)> Action;
	class GameActor* chaseTarget_ = nullptr;	// 이걸 모든 enemy가 가지고 있게 하고싶은데... 공통을 못뽑겠네 Next
	class SkellBoss* myPointer = nullptr;
	float appearAlpha = 0.0f;
	float waitCycle_ = 0.0f;
	Vector2 handGap = Vector2(0.0f,0.0f);
	bool attacked_ = false;	// 공격할 경우 true로.
	// 얘내둘은 공통
	float selectPatternWaitTime_ = 2.0f;	// 더 줄이면 bullet 버그남.
	int bulletCycle_ = 0;	// 얘는 각도 정확성을 위해 int
	float bulletRadian_ = 0.0f;
	// Bullet 기능
	class BossBullet* bullets_[100];
	Vector2 skelMousePos_ = Vector2(0.0f, 0.0f);// 총알상태 진입할때 skelMousePos 갱신
	float bulletDirection_ = 1.0f;
	int numOfActiveBullets_ = 0;
	int inactiveIndex = 0;
	// Sword 기능
	class BossSword* swords_[6];
	int numOfActiveSwords_ = 0;
	int numOfInactiveSwords_ = 1;		// EnterSWORD 시점에 값 할당
	
	// Laser 기능
	bool endLaser = false;	// 레이저 전용 상태 끝 알림
	bool beforeShootLaser = false;	// 방금 레이저 발사했는디 (SummonLaser 중복방지용 )
	class BossLaser* lasers_[2];	// 왼손용, 오른손용
	int minLaserCount_ = 1;
	int maxLaserCount_ = 3;	// 한번에 발사할 수 있는 레이저의 갯수 범위
	int FireLaserCount_ = 1;	// 레이저 상태 진입시, 레이저를 발사할 횟수 1 ~ 3
	int remainLaserCount_ = 1;	// 레이저가 완전히 소모될 때 -1 되어 wait상태로 진입하기 위한 remain 변수
	UINT laserDirection_ = 1;	// 0번이 왼손, 그러니 기본값은 반대가 되어야 0번이 왼쪽


private:
	void GenerateBullet();
public: // 생성자 & 소멸자
	SkellBoss();
	~SkellBoss();
public:	// 공개 인스턴스 변수 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	bool UpdateBulletCycle(int divFrame);	// 총알 여분 업데이트때문에 분리

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
public://Setter
	void DecreaseSword() { --numOfInactiveSwords_; }
	void DecreaseLaserCount() { --remainLaserCount_; if (remainLaserCount_ <= 0) endLaser = true; }
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
	void EnterBULLET();
	void ActionBULLET(Matrix V, Matrix P);

	void SwitchStateSWORD();
	void EnterSWORD();
	void ActionSWORD(Matrix V, Matrix P);

	void SwitchStateLASER();
	void EnterLASER();
	void ActionLASER(Matrix V, Matrix P);

	void SwitchStateDYING();
	void SwitchStateDEAD();

	void ActionDYING(Matrix V, Matrix P);
	void ActionDEAD(Matrix V, Matrix P);

	void EnterDYING();
	void EnterDEAD();		
};
