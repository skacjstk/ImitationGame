#pragma once
#include "./Object/Weapons/Weapon.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "./Base/GameActor.h"
class ShortSword : public MeleeWeapon
{
private:
	float attackDelay_ = 1.0f;	// Weapon�� ���ݼӵ��� ������� ���Ǵ� ���� ������
public:
	ShortSword();
	~ShortSword();
public:
	void Update(Matrix V, Matrix P) override;
	void Render()		            override;
	void Reset()		            override;
	void Fire()						override;
public:
	Weapon::WeaponType GetWeaponType() { return weaponType_; };
	// Setter
};
