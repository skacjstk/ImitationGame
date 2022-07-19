#include "./ImitationGame/framework.h"
#include "Scenes/Room.h"
#include "DungeonFactory.h"
#include "Floor_1.h"

Floor_1::Floor_1()
{
	SetActive(false);
	m_strSceneName = "Floor_1";
}

Floor_1::~Floor_1()
{
	SAFE_DELETE(backGround_);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 4; ++j) {
			SAFE_DELETE(roomData_[i][j]);
		}
	}
}

void Floor_1::Update()
{
	Matrix abV, P;
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();
//	backGround_->Update(abV, P);	// �� ������� ���
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Update();
}

void Floor_1::Render()
{
//	backGround_->Render();
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Render();
}

void Floor_1::ChangeScene()
{
	// 1. �� ����
	// 2. ���� ��ġ�� �� ��ġ ����
	GenerateRoom();
	// 3. �� ��ȿ�� Ȯ�� (  ���۰� ���� ����Ǿ��°�? )
	ConnectRoom(currentActiveRoom_[0], currentActiveRoom_[1]);
	// DFS �� ���� ���۰� ���� ����Ǿ��ִ��� Ȯ��. ( 0 �� �ƴϸ� �ϴ� ����� �� )
	// 4. Ư�� �� (1���� �ִ�) ����(����, �����Ա� ����)
	// ���߿� ����
	// 5. ��ȿ�� �˻�( ��� ���� Ž���� �� �ִ���? )
	// BFS ��� ���� �������� �������� Ž�� �������� �˻�
	// 6. �̸� ������� �� ����
	GenerateRoomData();
	// 6-1. �� ������ �ʿ��� �� �޸�
	/*
		���� ũ��, �����¿� ��Ż�� ��ġ�� ����� ���� ������
		���� ������ Gate ( ���� ������ ��Ż )�� ������ ������Ʈ ������
		���� Gate ����޾� �� �κ��� ���� ������ �ϼ��� ���� �̹��� ������
		�̹��� �����Ϳ� ����ȭ�� Line ������ ( �ϴ� ���� �������� )
		room ���� ���� ������ �㳪?
		���� ��ġ���� �־��ֱ�
	*/

}

void Floor_1::ExitScene()
{
}

void Floor_1::GenerateRoom()
{
	// ���� ��ġ�� �� ��ġ ����
	int SX;
	int SY;
	int EX;
	int EY;
	srand(time(NULL));
	while (true)
	{
		SX = rand() % 8;
		SY = rand() % 4;
		EX = rand() % 8;
		EY = rand() % 4;
		if (abs(SX - EX) + abs(SY - EY) > 6) {	// 6ĭ���� �ָ� ����
			SetSRER(SX, SY, EX, EY);
			currentActiveRoom_[0] = SX;
			currentActiveRoom_[1] = SY;
			break;	// ����
		}
	}
	// �� �� ���� ������ �̾����� ��.
	// ������ 2���� �迭�̰� ����갰���͵� ������ ��â�� �˰������� �ʿ䰡 ����.
	// �� �������� SX EY ������ ������.
	ConnectSRER(SX, SY, EX, EY);
}
// SX, SY�� ���� ��ġ��, EX, EY�� �� ��ġ�� �����Ѵ�. 
// out of range �˻簡 ������ ����.
void Floor_1::SetSRER(int& SX, int& SY, int& EX, int& EY)
{
	// 0 7, 0 3 ���� 
	tempRoom[SX][SY] = Room::RoomType::START;	// ���� ��
	tempRoom[EX][EY] = Room::RoomType::END;	// �� ��

	// ��¥ �� �����
	roomData_[SX][SY] = new Room(Room::RoomType::START);
	roomData_[EX][EY] = new Room(Room::RoomType::END);
}
// ���� ��ġ�� �� ��ġ�� �̾��ִ� �Լ�
// SX, SY�� �����ϴ� ���� ������ �ʿ��ϴٸ� ���� �Ҵ��س��� �Ѵ�.
void Floor_1::ConnectSRER(int& SX, int& SY, int& EX, int& EY)
{

	bool possibilityX = true;
	bool possibilityY = true;
	while (!(SX == EX) || !(SY == EY))
	{
		possibilityX = true;
		possibilityY = true;	// ���� ��ġ ���� �� ��ġ�� �����¿� ��� �Ӽ��� ������ �ִ��� �˻� ( d = distance )
		int dX = EX - SX;	// ������ left, ����� right
		int dY = SY - EY;    // ����� UP, ������ Down
		float diceX = 0.0f;
		float diceY = 0.0f;
		if ( dX == 0 )	// X�� Ȯ����� ����.
		{
			possibilityX = false;
		}
		if( dY == 0 ) // Y�� Ȯ����� ����.
		{
			possibilityY = false;
		}
		// �Ѵ� ���϶��� �� ������ Ȯ��������
		if (possibilityX == true || possibilityY == true) {
			// ��� ���ϸ� 0 �� �ǰ���?
			// ������ ������ �ȵǳ�?
		//	std::random_device rd;
		//	std::default_random_engine eng(rd());		
		//	std::uniform_real_distribution<int> dist(0, (abs(dX)) + (abs(dY)) - 1);

			int randn = rand() % ((abs(dX)) + (abs(dY)));
			printf("�Ÿ� ����:%d\n", (abs(dX)) + (abs(dY)));
			printf("��µ� random ��:%d\n", randn);
			if (randn - (abs(dY)) < 0) {
				// DY ��÷
				// Y�� �ø��ų� ����
				if (dY < 0)
					++SY;
				else
					--SY;
			}
			else {
				// DX ��÷
				// X�� �ø��ų� ����
				if (dX > 0)
					++SX;
				else
					--SX;
			}
			// ���� �ݿ� ���� �˻� ( ��ġ�� end�� �ȵ� )
			if (tempRoom[SX][SY] != (int)Room::RoomType::END) {
				printf("\n������ġ: %d %d\n", SX, SY);
				// ���� ��ġ�� �ƴϴ� �ش� ��ġ�� NORMAL�� ����
				tempRoom[SX][SY] = Room::RoomType::NORMAL;
				// ��¥ �� �����
				roomData_[SX][SY] = new Room(Room::RoomType::NORMAL);
			}			
			else // �� ����Ȱ���. ���۰� ���� �����ϱ�				
				break;
			
		} // end if
	}//end while
}

// �� �����ϱ� DFS ����� Ž�� �˰����� ( 
void Floor_1::ConnectRoom(int x, int y)
{
	// �̹� �湮�߰ų� End�̸� ��� return
	if (visitedRoom[x][y] == true)
		return;
	else
		visitedRoom[x][y] = true;

	if (tempRoom[x][y] == (int)Room::RoomType::END)
		return;

	// ���⸦ �������� Normal DFS ����
	if (tempRoom[x][y] != (int)Room::RoomType::DISABLE) {	// disable�� �ƴϸ�

		if (x+1 < maxX && tempRoom[x + 1][y] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::RIGHT);
			ConnectRoom(x + 1,y);
		}
		else if (x - 1 >= 0 && tempRoom[x - 1][y] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::LEFT);
			ConnectRoom(x - 1, y);
		}
		else if (y + 1 < maxY && tempRoom[x][y + 1] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::BOTTOM);
			ConnectRoom(x, y + 1);
		}
		else if (y - 1 >= 0 && tempRoom[x][y - 1] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::TOP);
			ConnectRoom(x, y - 1);
		}
	}		
}
// �̿ϼ�: ����ó�� Ž���ذ��� ���� �����͸� �������� �̹����� ���������.
void Floor_1::GenerateRoomData()
{
	// �ϴ� new ��ü�� ������ �Ǿ�����. �� �����̶� �׷���
	int x = currentActiveRoom_[0];
	int y = currentActiveRoom_[1];
	roomData_[x][y]->InitializeRoom();
	roomData_[x][y]->myIndex[0] = x;
	roomData_[x][y]->myIndex[1] = y;
}