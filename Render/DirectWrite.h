#pragma once

class DirectWrite
{
private:
	DirectWrite();
	~DirectWrite();

private:
	static DirectWrite* instance;

public:
	static void Create();
	static void Delete();
	static void BeginDraw();
	static void EndDraw();
	static DirectWrite* Get();
	static ID2D1DeviceContext* GetDC() { return deviceContext; }

	static void CreateBackBuffer();
	static void DeleteBackBuffer();

	static void RenderText(wstring& text, RECT& rect);
	static void RenderText(wstring text, Vector2 pos, float R, float G, float B, float size);
	static void DrawLine(Vector2 start, Vector2 end, float strokeWidth);
	static void SkewRectangle(RECT rect, D2D1::ColorF color, float angle, float angle2);
	static int  GetStringWidth(wstring text, float size);


private:
	ID2D1Factory1* factory; //2D�� Device�� �������ֱ� ����
	static IDWriteFactory* writeFactory; //DirectWrite����� �����ϱ� ����

	ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;

	static ID2D1Bitmap1* bitmap; //����ۿ�(D3D : Texture2D)
	static IDXGISurface* surface; //(D3D : RTV)

	static ID2D1SolidColorBrush* brush;  //�ؽ�Ʈ ����
	static IDWriteTextFormat*    format; //�ؽ�Ʈ ��Ʈ
	//static IDWriteTextFormat* formatLarge; //�ؽ�Ʈ ��Ʈ
	static bool m_bBeginDraw;
};