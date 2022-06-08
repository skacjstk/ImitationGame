#include "ImitationGame/framework.h"
#include "Keyboard.h"

// 0x0000 : ������ ���� ���� ���� ȣ�� �������� �ȴ��� ����
// 0x8000 : ������ ���� ���� ���� ȣ�� �������� ���� ����
// 0x8001 : ������ ���� ���� �ְ� ȣ�� �������� ���� ����
// 0x0001 : ������ ���� ���� �ְ� ȣ�� �������� �ȴ��� ����
/////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////
Keyboard::Keyboard()
{
	// �ʱ�ȭ
	for (int i = 0; i < KEYMAX; i++)
	{
		this->GetKeyUp().set(i, false);
		this->GetKeyDown().set(i, false);
	}
	
}

Keyboard::~Keyboard()
{
}

void Keyboard::WindowCallbackKeyDown(int key)
{
	//this->SetKeyDown(key, true);
}

void Keyboard::WindowCallbackKeyUp(int key)
{
	//this->SetKeyUp(key, true);
}

bool Keyboard::Down(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) // ������ �������� ���� ���
	{
		if (!this->GetKeyDown()[key])
		{
			this->SetKeyDown(key, true);
			return true;
		}

	}
	else
		this->SetKeyDown(key, false);
	return false;

// WinAPI
//	if (this->GetKeyDown()[key])
//	{
//		this->SetKeyDown(key, false);
//		return true;
//	}		
//	else
//		return false;

}

bool Keyboard::Up(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		this->SetKeyUp(key, true);
	else
	{
		if (this->GetKeyUp()[key])
		{
			this->SetKeyUp(key, false);
			return true;
		}
	}

	return false;

// WinAPI
//	if (this->GetKeyUp()[key])
//	{
//		this->SetKeyUp(key, false);
//		return true;
//	}	
//	else
//		return false;
}

bool Keyboard::Press(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool Keyboard::Toggle(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) return true;

	return false;
}
