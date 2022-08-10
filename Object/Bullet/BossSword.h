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
	Texture* bossSword_[2] = { nullptr, };	// 1번이 새하얀 상태: 발사중
	Animation* hitGroundFX_ = nullptr;
	class GameActor* chaseTarget_ = nullptr;
	float lifeTime_ = 0.0f;
	class SkellBoss** owner_ = nullptr;
private:
	int swordActiveIndex = 0;	// 0이 기본, 1이 활성화
	Vector2 movePos = Vector2(0.0f, 0.0f);
	float moveSpeed = 2400.0f;
	float bulletDamage_ = 4.0f;
	float waitTime_ = 0.0f;
	float chaseRad_ = 0.0f;	// 실제 각도가 아님
	int swordDyingFrame = 1;
public: // 생성자 & 소멸자
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

	void Dying() override;	// 총알이 죽어가며 FX 호출
	void Die() override;	// 완전히 죽어 false
};
