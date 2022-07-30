#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	std::array<wstring,50> enemyDB;	// �� enemy�� ���� �� �ƴ�. 
	std::array<int, 50> enemyNum;// ���°�� �����Ǵ��� Ȯ���ϴ� int�� �迭
	std::array<wstring, 50> NPCDB;	// �� NPC�� ���� �� �ƴ�.
	std::array<int, 50> npcNum;	// ���°�� �����Ǵ��� Ȯ���ϴ� int�� �迭

	Texture* FindActorTexture(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActor(int index);
	GameActor* FindActor(int index, wstring &objName, int & outObjIndex);
	void InitializeNumOneArray();
private:
	Texture* FindActorTextureOver100(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActorOver100(int index);
	GameActor* FindActorOver100(int index, wstring& objName, int& outObjIndex);
};