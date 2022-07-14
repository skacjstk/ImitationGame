#include "ImitationGame/framework.h"
#include "NPC.h"

NPC::NPC(int code)
{
	CreateNPC(code);
}

NPC::~NPC()
{
	SAFE_DELETE(_animation);
}

void NPC::CreateNPC(int code)
{
	// 대충 NPC코드 받아와 
	codeNPC_ = code;
}
