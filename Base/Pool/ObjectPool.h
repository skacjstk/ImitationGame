#pragma once
#define OBJECTPOOL ObjectPool::GetInstance()

class ObjectPool
{
public:

private:
	std::vector<Animation*> slashEffectPool_;	// ������ ����Ʈ ���� pool
	std::vector<Animation*> monsterDieEffectPool_;	// �� ������ ������ ����Ʈ
public:
	Animation** GetSlashEffect();	// �������� Die�� ���Ϳ��� �����͸� ������ �����ϰ� ����.
	Animation** GetMonsterDieEffect();
private:
	void AddSlashEffect();
	void AddMonsterDieEffect();

public:
	ObjectPool();
	~ObjectPool();

};
