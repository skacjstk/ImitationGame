#include "ImitationGame/framework.h"
#include "NPC.h"

NPC::NPC(int NPCID)
{
}

NPC::~NPC()
{
	SAFE_DELETE(_animation);
}
