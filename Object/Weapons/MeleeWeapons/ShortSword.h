#pragma once
#include "./Object/Weapons/Weapon.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "./Base/GameActor.h"
class ShortSword : public MeleeWeapon
{
private:
	Animation* attackFX_ = nullptr;
	Collider* attackCollider_ = nullptr;
	class GameActor* targetPointer_ = nullptr;	// �ش� Ÿ�� ���ƴ��� �˻��
public:
	ShortSword();
	~ShortSword();
public:
	void Update(Matrix V, Matrix P) override;
	void Render()		            override;
	void Reset()		            override;
	void Fire()						override;
	void CheckAttack();
public:
	Weapon::WeaponType GetWeaponType() { return weaponType_; };
	// Setter
private:
	void AnimationUpdate();
};
