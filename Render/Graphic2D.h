#pragma once
#include <gdiplus.h>
///////////////////////////////////////////
//  Class : 캡슐화, 정보은닉성, 다형성, 상속성, 추상화
//  
//  class내에 선언된 static 함수, static 변수는 C+에서
// 전역으로 사용된것 처럼 이용된다
//
// 1. GdiPlusStartUp(); 
// 2. CreateMemoryDC();
// 3. DrawImage를 계속 호출
// 4. Window 종료 될때 --> Shutdown 
////////////////////////////////////////////
class Graphic2D
{
public:
	void CreateMemoryDC(int wdith, int height);	// 화면에 직접 그리지 않고, backBuffer 이용함.
	void DrawImage(wstring strImageName, Vector2 position, Vector2 offset, Vector2 offsetSize);
	void SavePNG(wstring strFile);	// PNG파일로 Write 
	static void Merge();
	static void GdiPlusStartUp();
	static void GdiPlusShutDown();
private:

public:
	Graphic2D();
	~Graphic2D();
};

