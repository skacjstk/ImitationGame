#pragma once
#include "./Object/Weapons/Weapon.h"
class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon();
	~MeleeWeapon() {}
	virtual void Fire() = 0;
public:
	enum WeaponType GetWeaponType() { return weaponType_; }
};
