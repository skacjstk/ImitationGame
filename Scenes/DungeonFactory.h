#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
#include "Scenes/Room.h"
class DungeonFactory
{
public:
	static void GenerateDungeon(Room* room);	// 룸 껍데기는 있을 때 ( new 만 했을 때 )
	static Room* GenerateDungeon(int roomType) {};	// 룸이 아예 없을때 ( new 조차 안했을 때 )
};
