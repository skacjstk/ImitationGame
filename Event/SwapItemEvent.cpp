#include "ImitationGame/framework.h"
#include "Event.h"
#include "SwapItemEvent.h"

void SwapItemEvent::execute()
{
	printf("SwapItemEvnet Ãâ·Â");
	playerUI->equippedWeaponUI_->SwapItem();
}
