#pragma once
#include "./Object/Weapons/Weapon.h"
class MeleeWeapon : public Weapon
{
protected:
	Animation** slashEffect_ = nullptr;
public:
	MeleeWeapon();
	~MeleeWeapon() {}
	virtual void Fire() = 0;
protected:
	virtual void EffectUpdate(Matrix V, Matrix P);
	virtual void EffectRender();
public:
	enum WeaponType GetWeaponType() { return weaponType_; }
};
