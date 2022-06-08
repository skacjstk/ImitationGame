#pragma once
#include <gdiplus.h>
///////////////////////////////////////////
//  Class : ĸ��ȭ, �������м�, ������, ��Ӽ�, �߻�ȭ
//  
//  class���� ����� static �Լ�, static ������ C+����
// �������� ���Ȱ� ó�� �̿�ȴ�
//
// 1. GdiPlusStartUp(); 
// 2. CreateMemoryDC();
// 3. DrawImage�� ��� ȣ��
// 4. Window ���� �ɶ� --> Shutdown 
////////////////////////////////////////////
class Graphic2D
{
public:
	void CreateMemoryDC(int wdith, int height);	// ȭ�鿡 ���� �׸��� �ʰ�, backBuffer �̿���.
	void DrawImage(wstring strImageName, Vector2 position, Vector2 offset, Vector2 offsetSize);
	void SavePNG(wstring strFile);	// PNG���Ϸ� Write 
	static void Merge();
	static void GdiPlusStartUp();
	static void GdiPlusShutDown();
private:

public:
	Graphic2D();
	~Graphic2D();
};

