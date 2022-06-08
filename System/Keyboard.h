#pragma once
#define KEYMAX 256
#include <bitset>
#define  KEYBOARD  Keyboard::GetInstance()
class Keyboard
{
public:
	static Keyboard* GetInstance()
	{
		static Keyboard instance;
		return &instance;
	}

private:
	Keyboard();
	~Keyboard();

private:
	bitset<KEYMAX> keyUp;
	bitset<KEYMAX> keyDown;
	bitset<KEYMAX> GetKeyUp()   { return keyUp; }
	bitset<KEYMAX> GetKeyDown() { return keyDown; }
	void   SetKeyDown(int key, bool state) { keyDown.set(key, state); }
	void   SetKeyUp(int key, bool state)   { keyUp.set(key, state); }
	float  m_keyDelayTimer = 0.0f;   //  Ű�� ����ũ�� �Ҷ�  ��������
	float  m_StartTime = 0.0f;       //  Ű�� ����ũ�� �Ҷ�

public:
	bool   Down(int key);     // Key�� ������ �ִ°� Check
	bool   Up(int key);       // Key�� ����� �ִ°� Check
	bool   Press(int key);    // Key�� ��� ������ �ִ°� Check
	bool   Toggle(int key);
	void   WindowCallbackKeyDown(int key);
	void   WindowCallbackKeyUp(int key);
};

