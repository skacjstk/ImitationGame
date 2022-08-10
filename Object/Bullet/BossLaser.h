#pragma once
class BossLaser : public GameActor
{
public:
	enum class HandDirection {
		LEFT,
		RIGHT
	};
	enum class BLState {
		FIRE,	// 한참 쏘는 중 ( 콜라이더 활성화 )
		END	// 먼지조차 없다.
	};
public:
	BLState stateEnum_ = BLState::END;
	Animation* laserHead_ = nullptr;
	Animation* laserBody_[10] = { nullptr, };	// 1번이 새하얀 상태: 발사중, 이 바디들은 한줄기를 이루어야 함.

	class GameActor* chaseTarget_ = nullptr;
private:
	HandDirection fireDirection_ = HandDirection::LEFT;	// 손과 발사는 반대
	float damage_ = 4.0f;
	class SkellBoss** owner_ = nullptr;
public: // 생성자 & 소멸자
	BossLaser();
	~BossLaser();
public:
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	void  SummonLaser(SkellBoss** owner, Vector2 handPos, HandDirection direction, GameActor* chaseTarget);
private:
	void Fire(Matrix V, Matrix P);
	void Stop(Matrix V, Matrix P);
public:
	void  CheckAttack();
	BLState GetStateEnum() { return stateEnum_; }
};
