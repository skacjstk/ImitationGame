#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	std::array<wstring,50> enemyDB;	// �ε����� 
	std::array<wstring, 50> NPCDB;	// �ε����� 

	Texture* FindActorTexture(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActor(int index);
private:
	Texture* FindActorTextureOver100(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActorOver100(int index);
};