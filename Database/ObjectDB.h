#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	std::array<wstring,50> enemyDB;	// 인덱스에 
	std::array<wstring, 50> NPCDB;	// 인덱스에 

	Texture* FindActorTexture(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActor(int index);
private:
	Texture* FindActorTextureOver100(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActorOver100(int index);
};