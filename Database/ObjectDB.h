#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	wstring enemyDB[50];	// 인덱스에 
	wstring NPCDB[50];		// 인덱스에 

	Texture* FindActorTexture(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActor(int index);
private:
	Texture* FindActorTextureOver100(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActorOver100(int index);
};