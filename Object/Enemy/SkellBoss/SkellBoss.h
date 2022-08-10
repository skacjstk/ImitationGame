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
	eSkellBossState stateEnum_ = eSkellBossState::HIDE;	// �̳��� index ���. static_cast�� currentState_�� ������.
	Animation* hand_[2] = { nullptr, };	// IDLE, ATTACK ( LASER ) 0���� �޼�
	Animation* back_ = nullptr;	// �Ѿ� �� �޹��
	std::function<void()> SwitchState;
	std::function<void()> Enter;
	std::function<void(Matrix, Matrix)> Action;
	class GameActor* chaseTarget_ = nullptr;	// �̰� ��� enemy�� ������ �ְ� �ϰ������... ������ ���̰ڳ� Next
	class SkellBoss* myPointer = nullptr;
	float appearAlpha = 0.0f;
	float waitCycle_ = 0.0f;
	Vector2 handGap = Vector2(0.0f,0.0f);
	bool attacked_ = false;	// ������ ��� true��.
	// �곻���� ����
	float selectPatternWaitTime_ = 2.0f;	// �� ���̸� bullet ���׳�.
	int bulletCycle_ = 0;	// ��� ���� ��Ȯ���� ���� int
	float bulletRadian_ = 0.0f;
	// Bullet ���
	class BossBullet* bullets_[100];
	Vector2 skelMousePos_ = Vector2(0.0f, 0.0f);// �Ѿ˻��� �����Ҷ� skelMousePos ����
	float bulletDirection_ = 1.0f;
	int numOfActiveBullets_ = 0;
	int inactiveIndex = 0;
	// Sword ���
	class BossSword* swords_[6];
	int numOfActiveSwords_ = 0;
	int numOfInactiveSwords_ = 1;		// EnterSWORD ������ �� �Ҵ�
	
	// Laser ���
	bool endLaser = false;	// ������ ���� ���� �� �˸�
	bool beforeShootLaser = false;	// ��� ������ �߻��ߴµ� (SummonLaser �ߺ������� )
	class BossLaser* lasers_[2];	// �޼տ�, �����տ�
	int minLaserCount_ = 1;
	int maxLaserCount_ = 3;	// �ѹ��� �߻��� �� �ִ� �������� ���� ����
	int FireLaserCount_ = 1;	// ������ ���� ���Խ�, �������� �߻��� Ƚ�� 1 ~ 3
	int remainLaserCount_ = 1;	// �������� ������ �Ҹ�� �� -1 �Ǿ� wait���·� �����ϱ� ���� remain ����
	UINT laserDirection_ = 1;	// 0���� �޼�, �׷��� �⺻���� �ݴ밡 �Ǿ�� 0���� ����


private:
	void GenerateBullet();
public: // ������ & �Ҹ���
	SkellBoss();
	~SkellBoss();
public:	// ���� �ν��Ͻ� ���� 
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	bool UpdateBulletCycle(int divFrame);	// �Ѿ� ���� ������Ʈ������ �и�

public:	//Getter
	bool IsGround() { return isGround_; }
	bool IsConflicted() { return isConflicted_; }
public://Setter
	void DecreaseSword() { --numOfInactiveSwords_; }
	void DecreaseLaserCount() { --remainLaserCount_; if (remainLaserCount_ <= 0) endLaser = true; }
private:	// ���°�ü ��� ���� std::function<void()>
	bool CheckPlayer();	// �Ÿ� ����
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
