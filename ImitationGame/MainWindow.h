#pragma once
#define MAIN MainWindow::GetInstance()
class MainWindow
{
public:	//공개 스태틱 변수
	static MainWindow* GetInstance()
	{
		static MainWindow instance;
		return &instance;
	}
private:	//비공개 인스턴스 변수
	int _width = 0;
	int _height = 0;
	HINSTANCE _hInstnace = nullptr;
	HWND _hWnd = nullptr;

private:	//생성자와 소멸자
	MainWindow();
	~MainWindow();
public:	//공개	인스턴스	함수
	void Update();
	void Render();
	void Delete();
	void CreateInstance(HINSTANCE hInstnace, int Width, int Height);
	void CreateBackBuffer();	// Window Size 변경시
	void DeleteBackBuffer();	// Window Size 변경시
	void SetWindowSize(int width, int height);	// window size 변경시 
	void OnSize();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Getter
	int		GetWidth() { return _width; }
	int		GetHeight() { return _height; }
	HWND	GetWindowHandler() { return _hWnd; }
private://비공개	인스턴스	함수
	void Init3D();

};

