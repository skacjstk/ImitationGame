#include "ImitationGame/framework.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "ShortSword.h"

ShortSword::ShortSword()
{
	minDamage_ = 8;
	maxDamage_ = 10;
	weaponText_ = "������ �ֵθ��� ���� ��";
	handedType_ = Weapon::HandedType::ONEHANDED;
	if (static_cast<int>(handedType_) == 1)
		printf("static cast �Ѽչ���\n");
	if (handedType_ == Weapon::HandedType::ONEHANDED)
			printf("�Ѽչ���\n");
	printf("����Ÿ��: %d\n", static_cast<int>(weaponType_));
	attackDelay_ = 1 / attackSpeed_;	// ���� ���, speed��2 �� attackdelay�� 0.5�� �Ǿ�. 0.5�ʸ��� 1���� �����ϴ°���.

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
