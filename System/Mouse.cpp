#include "ImitationGame/framework.h"
#include "Mouse.h"

//////////////////////////////////////////////////////////
// ������
//////////////////////////////////////////////////////////
CMouse::CMouse(HWND handle)
	:handle(handle)
{
	this->handle = handle;
	ZeroMemory(buttonStatus,         sizeof(BYTE) * 8);     // Mouse Button ����
	ZeroMemory(buttonPrevStatus,     sizeof(BYTE) * 8);     // Mouse Button ����
	ZeroMemory(buttonMap,            sizeof(Button) * 8);   // 
	ZeroMemory(startDoubleClickTime, sizeof(DWORD) * 8);    // 
	ZeroMemory(buttonCount,          sizeof(int) * 8);      // 

	doubleClickTime = GetDoubleClickTime();  // �ý��� ���� Ŭ���ð�
	
//	doubleClickTime = 200;

}

CMouse::~CMouse()
{
}
void CMouse::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case   WM_MOUSEMOVE:
		   m_Position.x = LOWORD(lParam);
		   m_Position.y = HIWORD(lParam);
		   break;
	case   WM_MOUSEWHEEL:
		break;
	}
}

void CMouse::Update()
{
	// �ٸ�â���� ���콺�� ���������� ���ϱ� ���ؼ�
		//POINT  pt;
		//GetCursorPos(&pt);
		//ScreenToClient(handle, &pt);


	memcpy(this->buttonPrevStatus, this->buttonStatus, sizeof(BYTE) * 8);

	ZeroMemory(buttonStatus, sizeof(BYTE) * 8);
	ZeroMemory(buttonMap, sizeof(BYTE) * 8);

	// button status --> // 0x8000  ������ �������� ���� ȣ�� �������� ���� ����
	//  3~8����.... modify
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;

	for (UINT i = 0; i < 8; i++)
	{
		BYTE prevStatus     = buttonPrevStatus[i];
		BYTE CurrentStatus = buttonStatus[i];

		if (prevStatus == 0 && CurrentStatus == 1)
			buttonMap[i] = Button::Down;
		else if (prevStatus == 1 && CurrentStatus == 0)
			buttonMap[i] = Button::Up;
		else if (prevStatus == 1 && CurrentStatus == 1)
			buttonMap[i] = Button::Press;   // ��������� ������ ����
		else
			buttonMap[i] = Button::None;

	}

	// ����Ŭ��
	UINT currentTime = GetTickCount64();
	for (UINT i = 0; i < 8; i++)
	{
		if (buttonMap[i] == Button::Down)
		{
			if (buttonCount[i] == 1)
			{
				if (currentTime - startDoubleClickTime[i] >= doubleClickTime)
					buttonCount[i] = 0;

			}
			buttonCount[i]++;
			if (buttonCount[i] == 1)
				startDoubleClickTime[i] = currentTime;
		}

		if (buttonMap[i] == Button::Up)
		{
			if (buttonCount[i] == 1)
			{
				if (currentTime - startDoubleClickTime[i] >= doubleClickTime)
					buttonCount[i] = 0;

			}

			else if (buttonCount[i] == 2)
			{
				if (currentTime - startDoubleClickTime[i] <= doubleClickTime)
					buttonMap[i] = Button::DoubleClick;
				buttonCount[i] = 0;
			}

		}
	}



}

bool CMouse::Down(UINT button)
{
	// �ٸ�â���� ���콺�� ���������� ���ϱ� ���ؼ�

	HWND hwnd = GetFocus(); 

	if( hwnd == NULL)
		return false;

	if (hwnd != this->handle)
		return false;

	if (buttonMap[button] == Button::Down)
		return true;
	return false;
}

bool CMouse::Up(UINT button)
{
	return buttonMap[button] == Button::Up;
}

bool CMouse::Press(UINT button)
{
	return buttonMap[button] == Button::Press;
}

bool CMouse::DoubleClick(UINT button)
{
	return buttonMap[button] == Button::DoubleClick;
}
// 0 �� angle, 1�� radian, Ŀ�ø�: CAMERA->WCtoVC()
float CMouse::GetAngleRelativeToMouse(float x, float y, int viewType)
{
	float fAngle = 0.0f;
	Vector2 tempPosition = this->GetPosition();
	CAMERA->WCtoVC(tempPosition);


	float fdX = tempPosition.x - x;
	float fdY = tempPosition.y - y;
	float dRad = atan2f(fdY, fdX);
	if (viewType != 0)
		return dRad;	// ������ �������ϱ� 0�� �ƴϸ����� ����.

	fAngle = (dRad * 180.0f) / PI;
	return fAngle;
}


