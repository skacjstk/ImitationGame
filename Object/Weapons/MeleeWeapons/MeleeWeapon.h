#pragma once
#include "./Object/Weapons/Weapon.h"
class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon();
	~MeleeWeapon() {}
public:
	enum WeaponType GetWeaponType() { return weaponType_; }
};
