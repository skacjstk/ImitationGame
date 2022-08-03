#pragma once
#define OBJECTPOOL ObjectPool::GetInstance()

class ObjectPool
{
public:

private:
	std::vector<Animation*> slashEffectPool_;	// 슬래시 이펙트 관련 pool
	std::vector<Animation*> monsterDieEffectPool_;	// 몹 죽을때 나오는 이펙트
public:
	Animation** GetSlashEffect();	// 슬래쉬와 Die는 몬스터에서 포인터를 가져와 실행하게 하자.
	Animation** GetMonsterDieEffect();
private:
	void AddSlashEffect();
	void AddMonsterDieEffect();

public:
	ObjectPool();
	~ObjectPool();

};
