#include "./ImitationGame/framework.h"
#include "Scenes/DungeonFactory.h"
#include "Room.h"

Room::~Room()
{
	SAFE_DELETE(terrainImage_);
}

void Room::Update()
{
	printf("%d %d\n", myIndex[0], myIndex[1]);
}

void Room::Render()
{
}

void Room::ChangeScene()
{
}

void Room::ExitScene()
{
}
// 사실 오류검출 방법이 없어...
bool Room::InitializeRoom()
{
	bool value = false;

	DungeonFactory::GenerateDungeon(this);
	
	value = true;
	return value;
}

