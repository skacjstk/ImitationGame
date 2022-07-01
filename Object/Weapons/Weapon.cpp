#include "ImitationGame/framework.h"
#include "./Object/Item.h"
#include "Weapon.h"

Weapon::~Weapon()
{
	SAFE_DELETE(weapon_);
}