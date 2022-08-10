#pragma once
class BossSword : public GameActor
{
	enum class BState {
		CHASE,
		FIRE,
		STOP
	};
public:
	BState stateEnum_ = BState::STOP;
	Texture* bossSword_[2] = { nullptr, };	// 1���� ���Ͼ� ����: �߻���
	Animation* hitGroundFX_ = nullptr;
	class GameActor* chaseTarget_ = nullptr;
	float lifeTime_ = 0.0f;
	class SkellBoss** owner_ = nullptr;
private:
	int swordActiveIndex = 0;	// 0�� �⺻, 1�� Ȱ��ȭ
	Vector2 movePos = Vector2(0.0f, 0.0f);
	float moveSpeed = 2400.0f;
	float bulletDamage_ = 4.0f;
	float waitTime_ = 0.0f;
	float chaseRad_ = 0.0f;	// ���� ������ �ƴ�
	int swordDyingFrame = 1;
public: // ������ & �Ҹ���
	BossSword();
	~BossSword();
public:
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	void  SummonSword(SkellBoss** owner, Vector2& summonerPos, float waitTime, GameActor* chaseTarget);
	void  GroundCheck() override;
private:
	void Chasing(Matrix V, Matrix P);
	void Fire(Matrix V, Matrix P);
	void Stop(Matrix V, Matrix P);
public:
	void  CheckAttack();
	virtual void EffectUpdate(Matrix V, Matrix P) override;
	virtual void EffectRender() override;

	void Dying() override;	// �Ѿ��� �׾�� FX ȣ��
	void Die() override;	// ������ �׾� false
};
