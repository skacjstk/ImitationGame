#include "ImitationGame/framework.h"
#include "Object/NPC.h"
#include "Object/NPC/Dungeon.h"
#include "NPCPool.h"

NPCPool::NPCPool()
{
	npcList[DUNGEON] = new Dungeon();
}

NPCPool::~NPCPool()
{
	for(int i=0;i<_countof(npcList); ++i)
	SAFE_DELETE(npcList[i]);
}

NPC * NPCPool::GetNPC(int code)
{
	if (npcList[code] != nullptr)
		return npcList[code];
	else
		return nullptr;
}
