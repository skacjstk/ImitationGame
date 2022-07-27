#pragma once
class EnemyDB
{
public:
	EnemyDB();
public:
	vector<wstring> enemyDB;	// 인덱스에 

	Texture* FindActorTexture(int index);	// 해당 번호에 맞는 objectType을 골라줌.
	GameActor* FindActor(int index);
};