#include "ImitationGame/framework.h"
#include "Event.h"
#include "SwapItemEvent.h"

void SwapItemEvent::execute()
{
	printf("SwapItemEvnet ���");
	playerUI->equippedWeaponUI_->SwapItem();
}
