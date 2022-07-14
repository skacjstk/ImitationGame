#pragma once
#define NPCPOOL NPCPool::GetInstance()
class NPCPool
{
	enum codeNPC
	{
		DUNGEON = 0,
		BLACKSMITH = 1
	};
	// NPC들을 미리 만들어 담아놓는 객체
public:
	static NPCPool* GetInstance()
	{
		static NPCPool singleTon; // 생성이 두번되지는 않는 정적
		return &singleTon;
	}	
public:
	NPC* npcList[10] = { nullptr, };

public:
	NPCPool();
	~NPCPool();
public:
	NPC* GetNPC(int code);
};