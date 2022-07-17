#include "./ImitationGame/framework.h"
#include "Scenes/Room.h"
#include "DungeonFactory.h"
#include "Floor_1.h"

Floor_1::Floor_1()
{
	SetActive(false);
}

Floor_1::~Floor_1()
{
	SAFE_DELETE(backGround_);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j = 4; ++j) {
			SAFE_DELETE(roomData_[i][j]);
		}
	}
}

void Floor_1::Update()
{
	Matrix abV, P;
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	backGround_->Update(abV, P);	// �� �������
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Update();
}

void Floor_1::Render()
{
	backGround_->Render();
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Render();
}

void Floor_1::ChangeScene()
{
	// 1. �� ����
	// 2. ���� ��ġ�� �� ��ġ ����
	GenerateRoom();
	// 3. �� ��ȿ�� Ȯ�� (  ���۰� ���� ����Ǿ��°�? )
	// DFS �� ���� ���۰� ���� ����Ǿ��ִ��� Ȯ��. ( 0 �� �ƴϸ� �ϴ� ����� �� )
	// 4. Ư�� �� (1���� �ִ�) ����(����, �����Ա� ����)
	// 5. ��ȿ�� �˻�( ��� ���� Ž���� �� �ִ���? )
	// BFS ��� ���� �������� �������� Ž�� �������� �˻�
	// 6. �̸� ������� �� ����
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
	while (true)
	{
		SX = rand() % 8;
		SY = rand() % 4;
		EX = rand() % 8;
		EY = rand() % 4;
		if (abs(SX - EX) + abs(SY - EY) > 6) {	// 6ĭ���� �ָ� ����
			SetSRER(SX, SY, EX, EY);
			break;	// ����
		}
	}
	// �� �� ���� ������ �̾����� ��.
	// ������ 2���� �迭�̰� ����갰���͵� ������ ��â�� �˰����� �ʿ䰡 ����.
	// �� �������� SX EY ������ ������.
	ConnectSRER(SX, SY, EX, EY);
}
// SX, SY�� ���� ��ġ��, EX, EY�� �� ��ġ�� �����Ѵ�. 
// out of range �˻簡 ������ ����.
void Floor_1::SetSRER(int& SX, int& SY, int& EX, int& EY)
{
	// 0 7, 0 3 ���� 
	tempRoom[SX][SY] = RoomType::START;	// ���� ��
	tempRoom[EX][EY] = RoomType::END;	// �� ��
}
// ���� ��ġ�� �� ��ġ�� �̾��ִ� �Լ�
// SX, SY�� �����ϴ� ���� ������ �ʿ��ϴٸ� ���� �Ҵ��س��� �Ѵ�.
void Floor_1::ConnectSRER(int& SX, int& SY, int& EX, int& EY)
{	
	bool possibilityX = true;
	bool possibilityY = true;
	while (!(SX == EX) && !(SY == EY))
	{
		possibilityX = true;
		possibilityY = true;	// ���� ��ġ ���� �� ��ġ�� �����¿� ��� �Ӽ��� ������ �ִ��� �˻� ( d = distance )
		int dX = SX - EX;	// ����� left, ������ right
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

			std::random_device rd;
			std::default_random_engine eng(rd());

			std::uniform_real_distribution<int> dist(0, (abs(dX)) + (abs(dY)) - 1);
			if (dist(rd) - (abs(dY)) < 0) {
				// DY ��÷
				// Y�� �ø��ų� ����
				if (dY > 0)
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
			if (SY != EY || SX != EX) {
				// ���� ��ġ�� �ƴϴ� �ش� ��ġ�� NORMAL�� ����
				tempRoom[SX][SY] = RoomType::NORMAL;
			}			
			else // �� ����Ȱ���. ���۰� ���� �����ϱ�				
				break;
			
		} // end if
	}//end while
}
