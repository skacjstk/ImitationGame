#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	std::array<wstring,50> enemyDB;	// 꼭 enemy만 들어가는 건 아님. 
	std::array<wstring, 50> NPCDB;	// 꼭 NPC만 들어가는 건 아님.

	Texture* FindActorTexture(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActor(int index);
private:
	Texture* FindActorTextureOver100(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActorOver100(int index);
};