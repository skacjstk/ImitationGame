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
	// ���� NPC�ڵ� �޾ƿ� 
	codeNPC_ = code;
}
