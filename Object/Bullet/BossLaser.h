#pragma once
class BossLaser : public GameActor
{
public:
	enum class HandDirection {
		LEFT,
		RIGHT
	};
	enum class BLState {
		FIRE,	// ���� ��� �� ( �ݶ��̴� Ȱ��ȭ )
		END	// �������� ����.
	};
public:
	BLState stateEnum_ = BLState::END;
	Animation* laserHead_ = nullptr;
	Animation* laserBody_[10] = { nullptr, };	// 1���� ���Ͼ� ����: �߻���, �� �ٵ���� ���ٱ⸦ �̷��� ��.

	class GameActor* chaseTarget_ = nullptr;
private:
	HandDirection fireDirection_ = HandDirection::LEFT;	// �հ� �߻�� �ݴ�
	float damage_ = 4.0f;
	class SkellBoss** owner_ = nullptr;
public: // ������ & �Ҹ���
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
