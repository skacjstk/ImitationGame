#include "ImitationGame/framework.h"
#include "./Object/Item.h"
#include "Weapon.h"

Weapon::~Weapon()
{
	SAFE_DELETE(weapon_);
}
// 무기 최소 최대 피해 주사위 계산해줌. 실사용할땐 int로 변환할 것
float Weapon::DamageDice()
{
//    std::setprecision(3);
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(minDamage_, maxDamage_);
    std::cout << distr(eng) << "\n";
    return distr(eng);
}
