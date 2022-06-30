#include "ImitationGame/framework.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "ShortSword.h"

ShortSword::ShortSword()
{
	minDamage_ = 8;
	maxDamage_ = 10;
	ItemText_ = "가볍고 휘두르기 편한 검";
	handedType_ = Weapon::HandedType::ONEHANDED;
	if (static_cast<int>(handedType_) == 1)
		printf("static cast 한손무기\n");
	if (handedType_ == Weapon::HandedType::ONEHANDED)
			printf("한손무기\n");
	printf("무기타입: %d\n", static_cast<int>(weaponType_));
	attackDelay_ = 1 / attackSpeed_;	// 예를 들어, speed가2 면 attackdelay는 0.5가 되어. 0.5초마다 1번씩 공격하는거임.

//	pTexture_

}

ShortSword::~ShortSword()
{
//	SAFE_DELETE(pTexture_);
}

void ShortSword::Update(Matrix V, Matrix P)
{
}

void ShortSword::Render()
{
}

void ShortSword::Reset()
{
}
