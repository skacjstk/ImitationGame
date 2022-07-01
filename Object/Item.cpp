#include "ImitationGame/framework.h"
#include "Item.h"

Item::~Item()
{
	SAFE_DELETE(frontImage_);
}