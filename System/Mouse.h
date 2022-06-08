#pragma once
#define KEYMAX 256
////////////////////////////////////////////////////////////
// ���콺 Class
////////////////////////////////////////////////////////////
class CMouse
{
private:
	enum class Button{ None,Down,Up,Press,DoubleClick};

public:
	void		WndProc(UINT msg, WPARAM wParam, LPARAM lParam);  // MainWindow Callback()�Լ����� ����
	void		Update();

	bool		Down(UINT button);
	bool		Up(UINT button);
	bool		Press(UINT button);
	bool		DoubleClick(UINT button);
	Vector2		GetPosition() { return m_Position; }
private:
	HWND        handle;
	Vector2     m_Position = Vector2(0.0f,0.0f);

	BYTE		buttonStatus[8];
	BYTE		buttonPrevStatus[8];
	Button		buttonMap[8];
	DWORD		doubleClickTime;
	DWORD		startDoubleClickTime[8];
	int			buttonCount[8];


public: // ������/�Ҹ��� 
	CMouse(HWND handle);
	~CMouse();

};

