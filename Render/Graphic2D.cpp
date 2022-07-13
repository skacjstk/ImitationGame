#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "Graphic2D.h"
// Gdi Plus
// png파일을 사용하기 위해 
// 1. include
//    #include  <ole2.h>      --> object linking & embedding 기술
//    #include  <gdiplus.h>
// 2. library
//    #pragma comment(lib,"Gdiplus.lib")
// 3. Window Create된후  --> Startup
//   static ULONG_PTR gdiplusToken = NULL;
//	 GdiplusStartupInput gdiplusStartInput;
//   GdiplusStartup(&gdiplusToken, &gdiplusStartInput, NULL);
// 4. Window사 종료될때   --> Shudown
//   GdiplusShutdown(gdiplusToken);
//
#include   <ole2.h>  // Object Linking And Emedded : WinOS (OLE,Activex Com Object )
#include   <gdiplus.h>

#pragma  comment(lib,"Gdiplus.lib")
#pragma  comment(lib,"msimg32.lib")    // #pragma 컴파일 지시기

using namespace Gdiplus;		// 안쓰는게 더 좋아보이긴 해 
static ULONG_PTR gdiplusToken = NULL;

Gdiplus::Bitmap*    memBitmap = NULL;	// DirectX와 충돌 방지를 위해 namespace와 별개로 스코프 연산자 사용 
Gdiplus::Graphics*  memGC = NULL;

/////////////////////////////////////////////////////////
// 생성자/ 소멸자
/////////////////////////////////////////////////////////
Graphic2D::Graphic2D()
{
}

Graphic2D::~Graphic2D()
{
//	SAFE_DELETE(memBitmap);
//	SAFE_DELETE(memGC);
}

void Graphic2D::GdiPlusStartUp()
{
	GdiplusStartupInput  gdiplusStartInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartInput, NULL);
}

void Graphic2D::GdiPlusShutDown()
{
	GdiplusShutdown(gdiplusToken);
}
/*
화면
Bitmap 생성
MemDC 생성

*/
void Graphic2D::CreateMemoryDC(int width, int height)
{
	HDC dc = GetDC(MAIN->GetWindowHandler());
	Gdiplus::Graphics graphic(dc);

	// Bitmap & MemGraphic 생성
	memBitmap = new Gdiplus::Bitmap(width, height, &graphic);
	memGC = new Gdiplus::Graphics(memBitmap);

	Pen pen(Gdiplus::Color(0,0, 255, 0), 1.0f);
	SolidBrush brushFill(Gdiplus::Color(0, 2, 72, 57));	// a, r, g, b 순
//	pen.SetLineCap(LineCapRound, LineCapRound, DashCapFlat);

	// 투명한 Brush와 pen을 만들어 ,memDC 에 FillRectangle 함. 
	memGC->FillRectangle(&brushFill, 0, 0, width, height);


}
/*
IMG 파일을 읽어 mgmGC에 Rendering 
*/
void Graphic2D::DrawImage(wstring strImageName, Vector2 position, Vector2 offset, Vector2 offsetSize)
{
	Gdiplus::Image* images;

	images = new Gdiplus::Image(strImageName.c_str());

//	REAL  x = ( position.x + images->GetWidth()*0.5f);
//	REAL  y = ( position.y - images->GetHeight()*0.5f);
	// Update  Next 필요 
	// 1. offset, offsetSize
	// 2. Flip, Rotate
	// 3. scale 처리 
//	memGC->DrawImage(images, x, y, (float)images->GetWidth(), (float)images->GetHeight());	
	memGC->DrawImage(images, position.x, position.y, images->GetWidth(), images->GetHeight());
	SAFE_DELETE(images);
}
/*
 GdiPlus --> a 수정 가능
	r, g, b
	Png Write 
*/
void Graphic2D::SavePNG(wstring strFile)
{
	CLSID png;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &png);
	memBitmap->Save(strFile.c_str(), &png);
}




void Graphic2D::Merge()
{
//	return;//안쓸 땐 return해놓자

	HDC dc = GetDC(MAIN->GetWindowHandler());
	// DC 의 handle 을 가져와서 그래픽을 만들어준다.
	Graphics graphic(dc);
	// HDC 를 받아서 생성한다.

	Gdiplus::Bitmap* bit = new Gdiplus::Bitmap(100 * 28, 140, &graphic); // Bitmap 이미지 생성  그림 파일의 너비를 적어준다.
	Graphics* memG = new Graphics(bit);   // Memory DC 를 생성해준다. 여기에 그림을 그려서 파일을 만들어준다.

	int x = 0;
	Gdiplus::Image* images[30];

	for (int i = 1; i <= 28; i++)
	{
		/*wstring str = L"./Image/lucifer/lu_face";
		images[0] = new Gdiplus::Image(str.c_str());
		memG->DrawImage(images[0], 0, 0, images[0]->GetWidth(), images[0]->GetHeight());*/
		wstring str = IMAGE_FOLDER;
		str += L"Booper/booper (" + to_wstring(i) + L").png";//손 대려고하는 파일의 이름을 적어준다.
		images[i] = new Gdiplus::Image(str.c_str());
		memG->DrawImage(images[i], x, 0, images[i]->GetWidth(), images[i]->GetHeight());
		x = x + 100;

		SAFE_DELETE(images[i]);
	}

	wstring writeFileName = L"./_Texture/Image/Booper/booperAll.png";//만들어진 파일의 위치를 정해준다.

	CLSID png;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &png);
	bit->Save(writeFileName.c_str(), &png);
	delete bit;
	delete memG;

	return;
}
