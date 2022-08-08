#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	std::array<wstring,50> enemyDB;	// 꼭 enemy만 들어가는 건 아님. 
	std::array<int, 50> enemyNum;// 몇번째로 생성되는지 확인하는 int형 배열
	std::array<wstring, 50> NPCDB;	// 꼭 NPC만 들어가는 건 아님.
	std::array<int, 50> npcNum;	// 몇번째로 생성되는지 확인하는 int형 배열

	std::array<wstring, 20> bossDB;
	std::array<int, 20> bossNum;	// 몇번째로 생성되는지 확인하는 int형 배열

	Texture* FindActorTexture(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActor(int index, wstring &objName, int & outObjIndex);
	void InitializeNumOneArray();
private:
	Texture* FindActorTextureOver100(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActorOver100(int index);
	GameActor* FindActorOver100(int index, wstring& objName, int& outObjIndex);

	Texture* FindActorTextureOver1000(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActorOver1000(int index, wstring& objName, int& outObjIndex);
};