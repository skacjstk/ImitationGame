#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
#include "Object/NPC/Door.h"
#include "ObjectDB.h"

ObjectDB::ObjectDB()
{
	// �� �������� " ������Ʈ ��ġ ���" �ʰ� ������ �¾ƾ� ��. NONE ����, �׷��⿡ �̰����� �޾ƿö� -1 �ؼ� ã�ƾ� ��.
	// Template ��θ� ������ ����.
	enemyDB[0] = L"SkelDog";
	enemyDB[1] = L"BigWhiteSkel";
	enemyDB[2] = L"LittleGhost";

	// 100������ NPC
	NPCDB[0] = L"Door";
}
// �������� Texture ã�⿡���� switch���� ������ �� ����.
Texture* ObjectDB::FindActorTexture(int index)
{
	Texture* temp = nullptr;
	wstring strImage = IMAGE_FOLDER;
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	strImage += L"Monster/";

	if (index >= 101)
		return FindActorTexture(index);

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

GameActor* ObjectDB::FindActor(int index)
{
	GameActor* temp = nullptr;

	if (index >= 100)
		return FindActor(index);

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

Texture* ObjectDB::FindActorTextureOver100(int index)
{
	Texture* temp = nullptr;
	wstring strImage = IMAGE_FOLDER;
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	strImage += L"NPC/";

	// 101~�� NPC ����
	switch (index)
	{
	case 101:	
		strImage += NPCDB[index - 101] + L"/" + NPCDB[index - 101] + L"T.png";
		temp = new Texture(strImage, strShader);
		break;
	}
	return temp;
}

GameActor* ObjectDB::FindActorOver100(int index)
{
	GameActor* temp = nullptr;

	switch (index)
	{
	case 101:	// Door
		temp = (GameActor*)new Door();		// NPC�� �ᱹ Actor ��.
		break;
	}

	return temp;
}
