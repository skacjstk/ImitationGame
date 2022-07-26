#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "Graphic2D.h"
// Gdi Plus
// png������ ����ϱ� ���� 
// 1. include
//    #include  <ole2.h>      --> object linking & embedding ���
//    #include  <gdiplus.h>
// 2. library
//    #pragma comment(lib,"Gdiplus.lib")
// 3. Window Create����  --> Startup
//   static ULONG_PTR gdiplusToken = NULL;
//	 GdiplusStartupInput gdiplusStartInput;
//   GdiplusStartup(&gdiplusToken, &gdiplusStartInput, NULL);
// 4. Window�� ����ɶ�   --> Shudown
//   GdiplusShutdown(gdiplusToken);
//
#include   <ole2.h>  // Object Linking And Emedded : WinOS (OLE,Activex Com Object )
#include   <gdiplus.h>

#pragma  comment(lib,"Gdiplus.lib")
#pragma  comment(lib,"msimg32.lib")    // #pragma ������ ���ñ�

using namespace Gdiplus;		// �Ⱦ��°� �� ���ƺ��̱� �� 
static ULONG_PTR gdiplusToken = NULL;

Gdiplus::Bitmap*    memBitmap = NULL;	// DirectX�� �浹 ������ ���� namespace�� ������ ������ ������ ��� 
Gdiplus::Graphics*  memGC = NULL;

/////////////////////////////////////////////////////////
// ������/ �Ҹ���
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
ȭ��
Bitmap ����
MemDC ����

*/
void Graphic2D::CreateMemoryDC(int width, int height)
{
	HDC dc = GetDC(MAIN->GetWindowHandler());
	Gdiplus::Graphics graphic(dc);

	// Bitmap & MemGraphic ����
	memBitmap = new Gdiplus::Bitmap(width, height, &graphic);
	memGC = new Gdiplus::Graphics(memBitmap);

	Pen pen(Gdiplus::Color(0,0, 255, 0), 1.0f);
	SolidBrush brushFill(Gdiplus::Color(0, 2, 72, 57));	// a, r, g, b ��
//	pen.SetLineCap(LineCapRound, LineCapRound, DashCapFlat);

	// ������ Brush�� pen�� ����� ,memDC �� FillRectangle ��. 
	memGC->FillRectangle(&brushFill, 0, 0, width, height);


}
/*
IMG ������ �о� mgmGC�� Rendering 
*/
void Graphic2D::DrawImage(wstring strImageName, Vector2 position, Vector2 offset, Vector2 offsetSize)
{
	Gdiplus::Image* images;

	images = new Gdiplus::Image(strImageName.c_str());

//	REAL  x = ( position.x + images->GetWidth()*0.5f);
//	REAL  y = ( position.y - images->GetHeight()*0.5f);
	// Update  Next �ʿ� 
	// 1. offset, offsetSize
	// 2. Flip, Rotate
	// 3. scale ó�� 
//	memGC->DrawImage(images, x, y, (float)images->GetWidth(), (float)images->GetHeight());	
	memGC->DrawImage(images, position.x, position.y, images->GetWidth(), images->GetHeight());
	SAFE_DELETE(images);
}
/*
 GdiPlus --> a ���� ����
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
	return;//�Ⱦ� �� return�س���

	HDC dc = GetDC(MAIN->GetWindowHandler());
	// DC �� handle �� �����ͼ� �׷����� ������ش�.
	Graphics graphic(dc);
	// HDC �� �޾Ƽ� �����Ѵ�.

	Gdiplus::Bitmap* bit = new Gdiplus::Bitmap(320 * 3, 95, &graphic); // Bitmap �̹��� ����  �׸� ������ �ʺ� �����ش�.
	Graphics* memG = new Graphics(bit);   // Memory DC �� �������ش�. ���⿡ �׸��� �׷��� ������ ������ش�.
	//320 x 142	: ��
	//320 x 95 : ��
	int x = 0;
	Gdiplus::Image* images[30];

	for (int i = 0; i <= 2; i++)
	{
		/*wstring str = L"./Image/lucifer/lu_face";
		images[0] = new Gdiplus::Image(str.c_str());
		memG->DrawImage(images[0], 0, 0, images[0]->GetWidth(), images[0]->GetHeight());*/
		wstring str = L"../Resources/Image/Town/TownLayer_Day.png";//�� ������ϴ� ������ �̸��� �����ش�.
		
		images[i] = new Gdiplus::Image(str.c_str());
 		memG->DrawImage(images[i], x, 0, images[i]->GetWidth(), images[i]->GetHeight());
		x = x + 320;

		SAFE_DELETE(images[i]);
	}

	wstring writeFileName = L"../Resources/Image/Town/Modify.png";
		//������� ������ ��ġ�� �����ش�.

	CLSID png;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &png);
	bit->Save(writeFileName.c_str(), &png);
	delete bit;
	delete memG;

	return;
}
