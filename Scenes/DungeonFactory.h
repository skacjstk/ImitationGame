#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
#include "Scenes/Room.h"
class DungeonFactory
{
public:
	static void GenerateDungeon(Room* room);	// �� ������� ���� �� ( new �� ���� �� )
	static Room* GenerateDungeon(int roomType) {};	// ���� �ƿ� ������ ( new ���� ������ �� )
};
