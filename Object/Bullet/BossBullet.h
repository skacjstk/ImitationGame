#pragma once
class BossBullet : public GameActor
{
public:
	Animation* _animation = nullptr;
	Animation** bulletFX_ = nullptr;
	class GameActor* chaseTarget_ = nullptr;
	int lifeframe_ = 0;
private:
	Vector2 movePos = Vector2(0.0f, 0.0f);
	float moveSpeed = 5.0f;
	float bulletDamage_ = 3.0f;
public: // ������ & �Ҹ���
	BossBullet();
	~BossBullet();
public:
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	void  Fire(float radian, Vector2& skelMousePos, GameActor* chaseTarget);
	void  CheckAttack();
	virtual void EffectUpdate(Matrix V, Matrix P) override;
	virtual void EffectRender() override;

	void Dying() override;	// �Ѿ��� �׾�� FX ȣ��
	void Die() override;	// ������ �׾� false
};
