#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
#include "EnemyDB.h"

EnemyDB::EnemyDB()
{
	// 맵 에디터의 " 오브젝트 배치 모드" 쪽과 순서가 맞아야 함. NONE 제외, 그렇기에 이곳에서 받아올땐 -1 해서 찾아야 함.
	// Template 경로를 가지고 있음.
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
	// 너무 많아지면, 한 10개? 단위로 switch 문을 쪼갤 예정 0~9는 저쪽 함수, 10~19는 이쪽 함수 등등
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
