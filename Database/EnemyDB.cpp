#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
#include "EnemyDB.h"

EnemyDB::EnemyDB()
{
	// �� �������� " ������Ʈ ��ġ ���" �ʰ� ������ �¾ƾ� ��. NONE ����, �׷��⿡ �̰����� �޾ƿö� -1 �ؼ� ã�ƾ� ��.
	// Template ��θ� ������ ����.
	enemyDB.push_back(L"SkelDog");
	enemyDB.push_back(L"BigWhiteSkel");
	enemyDB.push_back(L"LittleGhost");
}

Texture* EnemyDB::FindActorTexture(int index)
{
	Texture* temp = nullptr;
	wstring strImage = IMAGE_FOLDER;
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	strImage += L"Monster/";
	// �ʹ� ��������, �� 10��? ������ switch ���� �ɰ� ���� 0~9�� ���� �Լ�, 10~19�� ���� �Լ� ���
	switch (index)
	{
	case 1:	// SkelDog
	case 2:	// BigWhiteSkel
	case 3:	// LittleGhost
		strImage += enemyDB[index - 1] + L"/" + enemyDB[index - 1] + L"T.png";
		temp = new Texture(strImage, strShader);
		break;
	}
	return temp;
}

GameActor* EnemyDB::FindActor(int index)
{
	GameActor* temp = nullptr;

	switch (index)
	{
	case 1:	// SkelDog
		temp = new SkelDog();
		break;
	case 2:	// BigWhiteSkel
		temp = new BigWhiteSkel();
		break;
	case 3:	// LittleGhost
		temp = new LittleGhost();
		break;
	}

	return temp;
}
