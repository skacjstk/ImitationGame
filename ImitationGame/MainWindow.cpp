#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "Resource.h"

//DX Cominterface �� Global
ID3D11Device*			Device = NULL;
ID3D11DeviceContext*	DeviceContext = NULL;
IDXGISwapChain*			SwapChain = NULL;
ID3D11RenderTargetView*	RTV = NULL;		//������ ���� ����
bool g_bVisibleCollisionCheck = false;

float WSCALEX = 1.0f;	// ������ ���� ����
float WSCALEY = 1.0f;	// ������ ���� ����

// ImGui ����(WndProc)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
	Delete();
}

void MainWindow::Delete()
{
	SAFE_RELEASE(Device);
	SAFE_RELEASE(DeviceContext);
	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(RTV);
}

void MainWindow::CreateInstance(HINSTANCE hInstance, int width, int height)
{
	_width = width;
	_height = height;
	wstring className = L"D2D";	//Ż��: lpszClassName ���⿡ �־��.
	//����� ������ Ŭ���� ��� 
	//https://blog.naver.com/makelove7942/220552981215   WNDCLASS ����
	{
		WNDCLASSEXW wcex;	//window Class ����ü 

		wcex.cbSize = sizeof(WNDCLASSEX);	//WNDCLASSEX(None Unicode)

		wcex.style = CS_HREDRAW | CS_VREDRAW; //OWNDC �� ��, ������ ���� �ɼ� (�� �ɼ��� ����|���� ���� ���� �����츦 �ٽ� �׸�
		wcex.lpfnWndProc = WndProc;	//*Callback ���ν��� : �����쿡�� �Ѹ� �޽����� ��Ƴ��ٰ� �Ѹ� ??  �ݺ� �Լ� �����͸� �����ϴ� �� -> �����쿡�� �߻��Ǵ� �޽����� ��� �Լ����� ó���� �� �޽��� ó���Լ��� �ּҸ� �޴´�.
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInstnace;	// ���α׷� ��ü �־��ֱ� 
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMITATIONGAME));	//�������� ������
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);	//�������� Ŀ�� 
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		if (width == 0 && height == 0)	// Ǯ��ũ������ �޴��� ����� ��.
			wcex.lpszMenuName = NULL;
		else
			wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IMITATIONGAME);
		wcex.lpszClassName = className.c_str();	//wcjar_t Ȯ�� ���ڿ� ��Ʈ(16��Ʈ)
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}
	//Window ���� 
	{
		_hInstnace = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		int dualW = GetSystemMetrics(SM_CXVIRTUALSCREEN);	// ��ü width
		int dualH = GetSystemMetrics(SM_CYVIRTUALSCREEN);	// ���� ū �κ� height
		int ScreenW = GetSystemMetrics(SM_CXSCREEN);		// ���� ��ũ�� width
		int offset = 0;
	//	if (dualW > ScreenW)
	//		offset = dualW - ScreenW;
		//�߿���. Ǯ��ũ�� ��� �߰�
		if (width == 0 && height == 0)
		{
			int ScreenX = GetSystemMetrics(SM_CXSCREEN);
			int ScreenY = GetSystemMetrics(SM_CYSCREEN);
			_hWnd = CreateWindowW(className.c_str(),
				L"16��", // * ����
				WS_POPUP,	// * ������ ��Ÿ��
				offset,			// * position X
				0,				// * position Y
				ScreenX,	// * nwidth
				ScreenY,				// * nHeight
				nullptr,	//parent Window (vs���� �ڵ� ��������, �θ�� ������ �ڽ��� ������ �� ����?
				nullptr,	//hMenu  (�޴��� �Ļ� �޴����� ������ �� ���� �÷��ִ� ��
				hInstance,  //���α׷��� ��ü 
				nullptr		//lpParam, Context(����), ��Ŭ���ϸ� ������ ����޴��� ���� ����. 
			);
		}
		else
		{
			_hWnd = CreateWindowW(className.c_str(),
				L"16��", // * ����
				dwStyle,	// * ������ ��Ÿ��
				offset,			// * position X
				0,				// * position Y
				_width,	// * nwidth
				_height,				// * nHeight
				nullptr,	//parent Window (vs���� �ڵ� ��������, �θ�� ������ �ڽ��� ������ �� ����?
				nullptr,	//hMenu  (�޴��� �Ļ� �޴����� ������ �� ���� �÷��ִ� ��
				hInstance,  //���α׷��� ��ü 
				nullptr		//lpParam, Context(����), ��Ŭ���ϸ� ������ ����޴��� ���� ����. 
			);
		}
		RECT rect;
		GetClientRect(MAIN->GetWindowHandler(), &rect);

		_width = rect.right - rect.left;
		_height = rect.bottom - rect.top;

		//		int GapX = width - abs(rect.right - rect.left);
		//		int GapY = height - abs(rect.bottom - rect.top);
		//		

		//	// ��������1��, ������ 2���� �����Դ� �ʿ䰡 ����.
		//
		//		printf("Screen: %d %d %d\n", dualW, dualH, ScreenW);
		//		printf("Gap: %d\t%d\t%d\n", GapX, GapY,offset);
		//		//������ ũ�� ���� �����(��¦ �߸��°� ����)
		//		MoveWindow(_hWnd, offset, 10, width + GapX, height + GapY, true);
	}
	// DriectX11 Setup
	Init3D();

	// Drag & Drop
	// �� â���� drop �Ǹ� png ���ϸ��� ��������
	// Callback �� ���� (WndProc)
	DragAcceptFiles(_hWnd, TRUE);	// <shellapi.h> �ʿ�

	ShowWindow(_hWnd, SW_SHOWDEFAULT); //������ �ڵ�, ���� flag (����)

	// FHD ���� Scale �� ���ϱ�.
	WSCALEX = _width / 1920.0f;
	WSCALEY = _height / 1080.0f;

	UpdateWindow(_hWnd);	//InvalidateRect()�� ó�� ������ �� ����.

}
/////////////////////////////////////////////////////////////////
//   2. SwapChain���� Backbuffer�� ����  CreateRenderTargetView�� �����Ѵ�
////////////////////////////////////////////////////////////////
void MainWindow::CreateBackBuffer()
{
	// Get Backbuffer
	ID3D11Texture2D* BackBuffer;
	// com interface --> typelib ( Automation���  --> Excel 
	HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));	// 1. SwapChain �� BackBuffer�� ������.

	// Create RenderTargetView
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV); // 2. �� BackBuffer �� RTV�� ������. 
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(BackBuffer);

	// ������� ���� Render() ������ �ִ� RTV ���ε� �κ� 
	// RenderTargetView -> OMSet  PipeLine Stage : IA->VS->PS->RS->OM 
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);	 // 3. �� RTV 1���� RenderTarget���� ������ 

	// Create ViewPort
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)_width;
		viewport.Height = (float)_height;

		DeviceContext->RSSetViewports(1, &viewport);	// ����Ʈ ���ε� 
	}
}

void MainWindow::DeleteBackBuffer()
{
	SAFE_RELEASE(RTV);
}
// ������ ũ�⸦ ���� ����.
void MainWindow::SetWindowSize(int width, int height)
{
	RECT rect = { 0,0,(LONG)width,(LONG)height };
	UINT centerX = GetSystemMetrics(SM_CXSCREEN) - width / 2;
	UINT centerY = GetSystemMetrics(SM_CYSCREEN) - height / 2;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow(_hWnd, 10, 10, rect.right - rect.left,
		rect.bottom - rect.top, TRUE);
}
// Callback �Լ����� Size ���ܿ����
// RTV �ٽ� ���� 
void MainWindow::OnSize()
{
	static int flag;
	flag++;
	// OnSize �߻� ����
	// 1. Create ���� ������		--> core dumpd �޻� ����
	// 2. Real Size ��¥�� size ���� ��
	// --> SwapChain 
	if (flag < 3) return;
	flag = 3;

	RECT rect;
	GetClientRect(this->GetWindowHandler(), &rect);

	_width = rect.right - rect.left;
	_height = rect.bottom - rect.top;

	if (Device != NULL)
	{
		//   ImGui::Invalidate();
		DeleteBackBuffer();
		//   DirectWrite::DeleteBackBuffer();

		HRESULT hr = SwapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
		//   DirectWrite::CreateBackBuffer();
		CreateBackBuffer();
		//   ImGui::Validate();
	}
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;	// ���⼭ �Ͼ �̺�Ʈ�� �Ʒ��� ������ �ʰڴ�. 
//	Mouse->WndProc(message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DROPFILES:
		hDrop = (HDROP)wParam;
//		if (SCENEMANAGER->GetCurrentScene()->GetName() == "MapEditor"){		
//			S04_Extra01* pScene = (S04_Extra01*)SCENEMANAGER->GetCurrentScene();
//			pScene->AddDrops(hDrop);
//		}
		break;
	case WM_SIZE:
		MAIN->OnSize();	// backBuffer �� �ٽ� ����
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MainWindow::Update()
{
	SCENEMANAGER->Update();
}

void MainWindow::Render()
{
	D3D11_VIEWPORT Viewport;
	{
		ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
		Viewport.TopLeftX = 0;	// ������ ������ (�»��)
		Viewport.TopLeftY = 0;	// ������ ������ (�»��)
		Viewport.Width = (float)this->GetWidth();
		Viewport.Height = (float)this->GetHeight();
	}

	// DX PipeLine
	// IA( input Assembly ) --> ������ ���� 
	// Vertex stage->Rasterizer stage->Pixel stage-> �̷� �͵��� �� �ذ����� OM(Output meger) �ܰ�
	// GPU�� �з��� ���μ���. ����� vertex, ������ pixel ���
	// �������� OM ���� �������� ������ ����
	// RTV�� �� �� ��� �Ѹ� ��

	//����Ÿ�� �並 Output meger�� ����Ÿ������ �����Ѵ�.
	DeviceContext->OMSetRenderTargets
		(1,				//����Ÿ���� ��
		&RTV,			//����Ÿ�� ���� �迭	(CreateRenderTargetView() ������ ����ߴ� 
		NULL			//����/���Ľ� ���� (���� ����)
		);	//OM Stage
	// ����Ʈ ���ε�
	DeviceContext->RSSetViewports
	(1,			//������ ����Ʈ�� ����
	&Viewport	//����Ʈ ����ü �迭
	);		//Rester Stage

	// BackGround Rendering 
	Color bgColor = Color(2.0f / 255.0f, 2.0f / 255.0f, 27.0f / 255.0f, 1.0f);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);	//-> �̰� ������ ������ �׷ȴ� �� ��� �����ǰ���?

	// Scene�� ���� Rendering �� �� �ְ�
	SCENEMANAGER->Render();
	SwapChain->Present(0, 0);	//����ü�� ǥ�� �ɼ� ���� SyncInterval, DXGI_PRESENT
}

//
// ���: DirectX11 �ʱ�ȭ
// 1. D3D11CreateDeviceAndSwapChain()�� �̿��Ͽ� 
//	like	Device(CPU ����)		: �ڿ��� �����ϴ� ����
//	DeviceContext(GPU)				: �׷���ī�� �����ؼ� �ϵ��� ó���ϴ� ����
//	SwapChain(BackBuffer)			: ������ü
//
//	--> comInterface���� �Ǿ� ����.
//		Ư¡ 
//		1) ���� �޸𸮸� ���� �Ҵ��� �� ����. --> ���� ������ Ÿ�� ���ο��� �ٷ����̺귯������ **�� �ּҸ� �޴� ��
//		���� �����ʹ� �³��� �˾Ƽ� ó��(new �����ٴ� ��)
//		2) ���� �� Release�Լ��� �̿��ؼ� �޸𸮸� �����ؾ� �Ѵ�.
//		3)	����Type�� "I" ��� ���ڰ� �׻� ����. (Interface��� ���)

//	2. SwapChain���� backbuffer�� ���� CreateRenderTargetView�� �����Ѵ�.
//
void MainWindow::Init3D()
{	
	HRESULT hr;
	// D3D11CreateDeviceAndSwapChain()�� �̿���	//SwapChain, Device, DeviceContext�� ���� 
	DXGI_MODE_DESC bufferDesc;
	{
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width = this->GetWidth();		// ȭ���� ��
		bufferDesc.Height = this->GetHeight();		// ȭ���� ����
		bufferDesc.RefreshRate.Numerator = 60;		// ���� �ֱ� ����
		bufferDesc.RefreshRate.Denominator = 1;		// ���� �ֱ� �и� (60/1, 1�ʿ� 60�� Refresh)
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �ȼ�����
		//UNORM(UINT Normalize : UINT �� ����ȭ 0~1) �ڷ����� ���� R8G8B8A8(8��Ʈ->1����Ʈ �� 4��) ���ڴ�
		//https://nickchooshin.tistory.com/6  �̺κ��� �� �̰� ����. ��� ���� ���ǵ� ����
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}
	//description �� ��� �� �� ZeroMemory�� �̿��ؼ� 0���� �ʱ�ȭ �Ͽ��� �Ѵ�.
	DXGI_SWAP_CHAIN_DESC swapDesc;
	{
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));	//����� ZeroMemory�� ���ο��� memset ��. �Ȱ�����
		swapDesc.BufferDesc = bufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ��¸��
		swapDesc.BufferCount = 1;		// SwapChain ���� ����� Back Buffer�� ���� (1��: ���� ���۸�)
		swapDesc.SampleDesc.Count = 1;	// �⺻�� (���ø�)(��Ƽ �ٸ����)
		swapDesc.SampleDesc.Quality = 0;	// �⺻��
		swapDesc.OutputWindow = MAIN->GetWindowHandler();	// ������ ��� ����� ������ �ڵ�
		swapDesc.Windowed = TRUE;			// True: â���, False�� â��� �ƴ�
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;		// Swap ȿ���� �����ϴ� ����ü
		//swap�� Front, back �Դٰ��� �Ѵٰ� ����? back���� front�� �ٲ� �� back �����͸� �����.�� ��
	}
	vector<D3D_FEATURE_LEVEL> feature_level =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	//������ �ϵ���� �켱������ Ȯ���� ����
	D3D_FEATURE_LEVEL currentFeatureLevel;
	//10167��° ��
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,	//__in_opt IDXGIAdapter*,	// ����ī�� ���� ���� (���� �Ⱦ��� �ϵ���� ��ī�� �ٸ�.)
		D3D_DRIVER_TYPE_HARDWARE, //D3D_DRIVER_TYPE	// HARDWARE�� ��ī ���� ��. F12�� Ȯ��
		NULL,	//HMODULE,	// DLL (Thrid Party) ex) OpenGL SGI ������ ���̾��� ���� ���
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, //UINT	3D + 2D ȥ���� �ɼ�
		feature_level.data(),	// CONST D3D_FEATURE_LEVEL*	// ���� ������ ���� ���ϱ� (��ī �������� �Ʒ���: ��������)
		(UINT)feature_level.size(),	// UINT FeatureLevels // ���ǵ� ����
		D3D11_SDK_VERSION,		//UINT SDKVersion // SDK ����
		&swapDesc,//		DXGI_SWAP_CHAIN_DESC* // swapchain description
		&SwapChain, // IDXGISwapChain** //�����Ǵ� SwapChain  Out �Ķ����
		&Device, // ID3D11Device**	// �����Ǵ� Device  Out �Ķ����
		&currentFeatureLevel,//  D3D_FEATURE_LEVEL* // � �� ���Դ��� Ȯ���غ������ ��	Out �Ķ���� 
		&DeviceContext // ID3D11DeviceContext** // �����Ǵ� DeviceContext	Out �Ķ����
	);

	assert(SUCCEEDED(hr));

	// 2. SwapChain���� backBuffer�� ���� CreateRenderTargetView�� �����Ѵ�.
	/*
	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));

	//�޴��� �Ǽ� 2�а� �а� �ڵ��ϼ���. (�Ȱ��ٴ� ��), �� �κ��� CreatebackBuffer() �� ����
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(BackBuffer);
	
	SCENEMANAGER->GetInstance();
	*/
	CreateBackBuffer();
}