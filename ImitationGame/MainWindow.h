#pragma once
#define MAIN MainWindow::GetInstance()
class MainWindow
{
public:	//���� ����ƽ ����
	static MainWindow* GetInstance()
	{
		static MainWindow instance;
		return &instance;
	}
private:	//����� �ν��Ͻ� ����
	int _width = 0;
	int _height = 0;
	HINSTANCE _hInstnace = nullptr;
	HWND _hWnd = nullptr;
	HWND _introVideo = nullptr;
	bool _stopVideo = false;	// ������� ���� ����

private:	//�����ڿ� �Ҹ���
	MainWindow();
	~MainWindow();
public:	//����	�ν��Ͻ�	�Լ�
	void Update();
	void Render();
	void Delete();
	void CreateInstance(HINSTANCE hInstnace, int Width, int Height);
	void CreateBackBuffer();	// Window Size �����
	void DeleteBackBuffer();	// Window Size �����
	void SetWindowSize(int width, int height);	// window size ����� 
	void OnSize();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Getter
	int		GetWidth() { return _width; }
	int		GetHeight() { return _height; }
	HWND	GetWindowHandler() { return _hWnd; }
private://�����	�ν��Ͻ�	�Լ�
	void Init3D();
	void CreateMCIWindow();

};

