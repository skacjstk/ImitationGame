#pragma once
#include "./Object/Weapons/Weapon.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "./Base/GameActor.h"
class ShortSword : public MeleeWeapon
{
private:
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
private:
	void AnimationUpdate();
};
