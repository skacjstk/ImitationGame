#pragma once
class EnemyDB
{
public:
	EnemyDB();
public:
	vector<wstring> enemyDB;	// �ε����� 

	Texture* FindActorTexture(int index);	// �ش� ��ȣ�� �´� objectType�� �����.
	GameActor* FindActor(int index);
};