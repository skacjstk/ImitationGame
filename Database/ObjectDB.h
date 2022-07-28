#pragma once
class ObjectDB
{
public:
	ObjectDB();
public:
	wstring enemyDB[50];	// �ε����� 
	wstring NPCDB[50];		// �ε����� 

	Texture* FindActorTexture(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActor(int index);
private:
	Texture* FindActorTextureOver100(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActorOver100(int index);
};