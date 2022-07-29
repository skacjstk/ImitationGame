#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	std::array<wstring,50> enemyDB;	// �� enemy�� ���� �� �ƴ�. 
	std::array<wstring, 50> NPCDB;	// �� NPC�� ���� �� �ƴ�.

	Texture* FindActorTexture(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActor(int index);
private:
	Texture* FindActorTextureOver100(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActorOver100(int index);
};