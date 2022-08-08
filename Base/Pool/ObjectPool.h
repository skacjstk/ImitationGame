#pragma once
#define OBJECTPOOL ObjectPool::GetInstance()

class ObjectPool
{
public:

private:
	std::vector<Animation*> slashEffectPool_;	// ������ ����Ʈ ���� pool
	std::vector<Animation*> monsterDieEffectPool_;	// �� ������ ������ ����Ʈ
	std::vector<Animation*> bossBulletDisableEffectPool_;	// BossBullet ������� ȣ���� ����Ʈ
public:
	Animation** GetSlashEffect();	// �������� Die�� ���Ϳ��� �����͸� ������ �����ϰ� ����.
	Animation** GetMonsterDieEffect();
	Animation** GetBossBulletEffect();
private:
	void AddSlashEffect();
	void AddMonsterDieEffect();
	void AddBossBulletEffect();

public:
	ObjectPool();
	~ObjectPool();

};
