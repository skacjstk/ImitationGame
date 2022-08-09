#include "ImitationGame/framework.h"
#include "./Object/Item.h"
#include "Weapon.h"

Weapon::~Weapon()
{
	SAFE_DELETE(weapon_);
}
// ���� �ּ� �ִ� ���� �ֻ��� �������. �ǻ���Ҷ� int�� ��ȯ�� ��
float Weapon::DamageDice()
{
//    std::setprecision(3);
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(minDamage_, maxDamage_);
 //   std::cout << distr(eng) << "\n";
    return distr(eng);
}
