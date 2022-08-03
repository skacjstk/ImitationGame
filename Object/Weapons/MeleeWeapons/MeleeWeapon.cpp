#include "ImitationGame/framework.h"
#include "./Object/Weapons/Weapon.h"
#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon()
{
	weaponType_ = Weapon::WeaponType::MELEE;
}

void MeleeWeapon::EffectUpdate(Matrix V, Matrix P)
{
	if (slashEffect_ != nullptr) {
		if ((*slashEffect_)->IsPlay() == true)
			(*slashEffect_)->Update(V, P);
		else
		{
			slashEffect_ = nullptr;
		}
	}
}

void MeleeWeapon::EffectRender()
{
	if (slashEffect_ != nullptr)
		(*slashEffect_)->Render();
}
