#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
#include "Object/NPC/Door.h"
#include "Object/TileObject/Stele.h"	// 일단은 NPCDB 에
#include "Object/Enemy/SkellBoss/SkellBoss.h"
#include "ObjectDB.h"

ObjectDB::ObjectDB()
{
	// 맵 에디터의 " 오브젝트 배치 모드" 쪽과 순서가 맞아야 함. NONE 제외, 그렇기에 이곳에서 받아올땐 -1 해서 찾아야 함.
	// Template 경로를 가지고 있음.
	enemyDB[0] = L"SkelDog";
	enemyDB[1] = L"BigWhiteSkel";
	enemyDB[2] = L"LittleGhost";

	// 100번부터 NPC
	NPCDB[0] = L"Door";
	NPCDB[1] = L"Stele";

	bossDB[0] = L"SkellBoss";

	InitializeNumOneArray();
}
// 많아지면 Texture 찾기에서는 switch문이 없어질 수 있음.
Texture* ObjectDB::FindActorTexture(int index)
{
	if (index >= 1001)	// 내가 왜 여기만 1 더 작게 했었지? 100, 1000 등
		return FindActorTextureOver1000(index);
	if (index >= 101)
		return FindActorTextureOver100(index);

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
		strImage += enemyDB.at(index - 1) + L"/" + enemyDB.at(index - 1) + L"T.png";
		temp = new Texture(strImage, strShader);
		break;
	}
	return temp;
}

GameActor* ObjectDB::FindActor(int index, wstring& objName, int& outObjIndex)
{
	if (index >= 1001)
		return FindActorOver1000(index, objName, outObjIndex);
	if (index >= 101)
		return FindActorOver100(index, objName, outObjIndex);
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
	if (temp != nullptr) {
		objName = enemyDB[index - (size_t)1];
		outObjIndex = enemyNum[(index - (size_t)1)]++;	// 이러면 넣고 더해지려나
	}

	return temp;
}

Texture* ObjectDB::FindActorTextureOver100(int index)
{
	Texture* temp = nullptr;
	wstring strImage = IMAGE_FOLDER;
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	strImage += L"NPC/";

	// 101~은 NPC 영역
	switch (index)
	{
	case 101:
	case 102:	// stele
		strImage += NPCDB[index - (size_t)101] + L"/" + NPCDB[index - (size_t)101] + L"T.png";
		temp = new Texture(strImage, strShader);
		break;
	}
	return temp;
}

void ObjectDB::InitializeNumOneArray()
{
	enemyNum.fill(1);
	npcNum.fill(1);
}

GameActor* ObjectDB::FindActorOver100(int index)
{
	GameActor* temp = nullptr;

	switch (index)
	{
	case 101:	// Door
		temp = (GameActor*)new Door();		// NPC도 결국 Actor 임.
		break;
	case 102:	// Stele
		temp = (GameActor*)new Stele();		 
		break;
	}

	return temp;
}

GameActor* ObjectDB::FindActorOver100(int index, wstring& objName, int& outObjIndex)
{
	GameActor* temp = nullptr;

	switch (index)
	{
	case 101:	// Door
		temp = (GameActor*)new Door();		// NPC도 결국 Actor 임.
		break;
	case 102:	// Stele
		temp = (GameActor*)new Stele();		// 
		break;
	}

	if (temp != nullptr) {
		objName = NPCDB[index - (size_t)101];
		outObjIndex = npcNum[index - (size_t)101]++;	// 이러면 넣고 더해지려나
	}

	return temp;
}

Texture* ObjectDB::FindActorTextureOver1000(int index)
{
	Texture* temp = nullptr;
	wstring strImage = IMAGE_FOLDER;
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	strImage += L"Boss/";

	// 101~은 NPC 영역
	switch (index)
	{
	case 1001:	// 
		strImage += bossDB[index - (size_t)1001] + L"/" + bossDB[index - (size_t)1001] + L"T.png";
		temp = new Texture(strImage, strShader);
		break;
	}
	return temp;
}

GameActor* ObjectDB::FindActorOver1000(int index, wstring& objName, int& outObjIndex)
{
	GameActor* temp = nullptr;

	switch (index)
	{
	case 1001:	// SkellBoss
		temp = (GameActor*)new SkellBoss();	
		break;
	}

	if (temp != nullptr) {
		objName = bossDB[index - (size_t)1001];
		outObjIndex = bossNum[index - (size_t)1001]++;	// 이러면 넣고 더해지려나
	}

	return temp;
}
