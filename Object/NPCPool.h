#pragma once
#define NPCPOOL NPCPool::GetInstance()
class NPCPool
{
	enum codeNPC
	{
		DUNGEON = 0,
		BLACKSMITH = 1
	};
	// NPC���� �̸� ����� ��Ƴ��� ��ü
public:
	static NPCPool* GetInstance()
	{
		static NPCPool singleTon; // ������ �ι������� �ʴ� ����
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