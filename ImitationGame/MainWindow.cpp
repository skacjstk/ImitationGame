#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "Resource.h"

//DX Cominterface 및 Global
ID3D11Device*			Device = NULL;
ID3D11DeviceContext*	DeviceContext = NULL;
IDXGISwapChain*			SwapChain = NULL;
ID3D11RenderTargetView*	RTV = NULL;		//실제로 보는 영역
bool g_bVisibleCollisionCheck = true;

float WSCALEX = 1.0f;	// 오프셋 배율 계산용
float WSCALEY = 1.0f;	// 오프셋 배율 계산용
const float G = 9.81f;	// 중력상수

// ImGui 변수(WndProc)
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
	wstring className = L"D2D";	//탈선: lpszClassName 여기에 넣어둠.
	//사용할 윈도우 클래스 등록 
	//https://blog.naver.com/makelove7942/220552981215   WNDCLASS 설명
	{
		WNDCLASSEXW wcex;	//window Class 구조체 

		wcex.cbSize = sizeof(WNDCLASSEX);	//WNDCLASSEX(None Unicode)

		wcex.style = CS_HREDRAW | CS_VREDRAW; //OWNDC 좀 씀, 윈도우 생성 옵션 (이 옵션은 수평|수직 폭에 대해 윈도우를 다시 그림
		wcex.lpfnWndProc = WndProc;	//*Callback 프로시져 : 윈도우에서 뿌린 메시지들 담아놨다가 뿌림 ??  콜벡 함수 포인터를 저장하는 것 -> 윈도우에서 발생되는 메시지를 어느 함수에서 처리할 지 메시지 처리함수의 주소를 받는다.
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInstnace;	// 프로그램 객체 넣어주기 
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMITATIONGAME));	//실행파일 아이콘
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);	//실행파일 커서 
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		if (width == 0 && height == 0)	// 풀스크린에서 메뉴바 없어야 함.
			wcex.lpszMenuName = NULL;
		else
			wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IMITATIONGAME);
		wcex.lpszClassName = className.c_str();	//wcjar_t 확장 문자열 세트(16비트)
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}
	//Window 생성 
	{
		_hInstnace = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		int dualW = GetSystemMetrics(SM_CXVIRTUALSCREEN);	// 전체 width
		int dualH = GetSystemMetrics(SM_CYVIRTUALSCREEN);	// 가장 큰 부분 height
		int ScreenW = GetSystemMetrics(SM_CXSCREEN);		// 현재 스크린 width
		int offset = 0;
	//	if (dualW > ScreenW)
	//		offset = dualW - ScreenW;
		//중요함. 풀스크린 경우 추가
		if (width == 0 && height == 0)
		{
			int ScreenX = GetSystemMetrics(SM_CXSCREEN);
			int ScreenY = GetSystemMetrics(SM_CYSCREEN);
			_hWnd = CreateWindowW(className.c_str(),
				L"16반", // * 제목
				WS_POPUP,	// * 윈도우 스타일
				offset,			// * position X
				0,				// * position Y
				ScreenX,	// * nwidth
				ScreenY,				// * nHeight
				nullptr,	//parent Window (vs에서 코드 떨어질때, 부모는 누구고 자식이 누군지 감 오지?
				nullptr,	//hMenu  (메뉴바 파생 메뉴들을 생성할 때 같이 올려주는 곳
				hInstance,  //프로그램의 실체 
				nullptr		//lpParam, Context(문맥), 우클릭하면 나오는 서브메뉴에 대한 정보. 
			);
		}
		else
		{
			_hWnd = CreateWindowW(className.c_str(),
				L"16반", // * 제목
				dwStyle,	// * 윈도우 스타일
				offset,			// * position X
				0,				// * position Y
				_width,	// * nwidth
				_height,				// * nHeight
				nullptr,	//parent Window (vs에서 코드 떨어질때, 부모는 누구고 자식이 누군지 감 오지?
				nullptr,	//hMenu  (메뉴바 파생 메뉴들을 생성할 때 같이 올려주는 곳
				hInstance,  //프로그램의 실체 
				nullptr		//lpParam, Context(문맥), 우클릭하면 나오는 서브메뉴에 대한 정보. 
			);
		}
		RECT rect;
		GetClientRect(MAIN->GetWindowHandler(), &rect);

		_width = rect.right - rect.left;
		_height = rect.bottom - rect.top;

		//		int GapX = width - abs(rect.right - rect.left);
		//		int GapY = height - abs(rect.bottom - rect.top);
		//		

		//	// 오른쪽이1번, 왼쪽이 2번인 나에게는 필요가 없음.
		//
		//		printf("Screen: %d %d %d\n", dualW, dualH, ScreenW);
		//		printf("Gap: %d\t%d\t%d\n", GapX, GapY,offset);
		//		//윈도우 크기 재대로 만들기(살짝 잘리는거 복구)
		//		MoveWindow(_hWnd, offset, 10, width + GapX, height + GapY, true);
	}
	// DriectX11 Setup
	Init3D();

	// Drag & Drop
	// 내 창으로 drop 되면 png 파일명을 가져오게
	// Callback 을 통해 (WndProc)
	DragAcceptFiles(_hWnd, TRUE);	// <shellapi.h> 필요

	ShowWindow(_hWnd, SW_SHOWDEFAULT); //윈도우 핸들, 상태 flag (정수)

	// FHD 기준 Scale 값 구하기.
	WSCALEX = _width / 1920.0f;
	WSCALEY = _height / 1080.0f;

	UpdateWindow(_hWnd);	//InvalidateRect()는 처리 지연될 수 있음.

}
/////////////////////////////////////////////////////////////////
//   2. SwapChain으로 Backbuffer를 얻어와  CreateRenderTargetView를 생성한다
////////////////////////////////////////////////////////////////
void MainWindow::CreateBackBuffer()
{
	// Get Backbuffer
	ID3D11Texture2D* BackBuffer;
	// com interface --> typelib ( Automation기법  --> Excel 
	HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));	// 1. SwapChain 에 BackBuffer를 가져와.

	// Create RenderTargetView
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV); // 2. 그 BackBuffer 를 RTV를 생성해. 
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(BackBuffer);

	// 여기부터 원래 Render() 시점에 있던 RTV 바인딩 부분 
	// RenderTargetView -> OMSet  PipeLine Stage : IA->VS->PS->RS->OM 
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);	 // 3. 그 RTV 1개를 RenderTarget으로 설정해 

	// Create ViewPort
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)_width;
		viewport.Height = (float)_height;

		DeviceContext->RSSetViewports(1, &viewport);	// 뷰포트 바인딩 
	}
}

void MainWindow::DeleteBackBuffer()
{
	SAFE_RELEASE(RTV);
}
// 윈도우 크기를 새로 설정.
void MainWindow::SetWindowSize(int width, int height)
{
	RECT rect = { 0,0,(LONG)width,(LONG)height };
	UINT centerX = GetSystemMetrics(SM_CXSCREEN) - width / 2;
	UINT centerY = GetSystemMetrics(SM_CYSCREEN) - height / 2;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow(_hWnd, 10, 10, rect.right - rect.left,
		rect.bottom - rect.top, TRUE);
}
// Callback 함수에서 Size 변겨오디면
// RTV 다시 생성 
void MainWindow::OnSize()
{
	static int flag;
	flag++;
	// OnSize 발생 조건
	// 1. Create 최초 생성시		--> core dumpd 달생 가능
	// 2. Real Size 진짜로 size 변경 시
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
		return true;	// 여기서 일어난 이벤트는 아래로 보내지 않겠다. 
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
		MAIN->OnSize();	// backBuffer 를 다시 생성
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
		Viewport.TopLeftX = 0;	// 윈도우 시작점 (좌상단)
		Viewport.TopLeftY = 0;	// 윈도우 시작점 (좌상단)
		Viewport.Width = (float)this->GetWidth();
		Viewport.Height = (float)this->GetHeight();
	}

	// DX PipeLine
	// IA( input Assembly ) --> 정점을 생성 
	// Vertex stage->Rasterizer stage->Pixel stage-> 이런 것들을 다 해가지고 OM(Output meger) 단계
	// GPU는 패러랠 프로세싱. 여기는 vertex, 저쪽은 pixel 등등
	// 마지막에 OM 으로 합쳐져서 랜더링 해줌
	// RTV는 그 거 어디에 뿌릴 지

	//렌더타겟 뷰를 Output meger의 렌더타겟으로 설정한다.
	DeviceContext->OMSetRenderTargets
		(1,				//렌더타겟의 수
		&RTV,			//렌더타겟 뷰의 배열	(CreateRenderTargetView() 시점에 등록했던 
		NULL			//깊이/스탠실 버퍼 (현재 없음)
		);	//OM Stage
	// 뷰포트 바인딩
	DeviceContext->RSSetViewports
	(1,			//설정할 뷰포트의 개수
	&Viewport	//뷰포트 구조체 배열
	);		//Rester Stage

	// BackGround Rendering 
	Color bgColor = Color(2.0f / 255.0f, 2.0f / 255.0f, 27.0f / 255.0f, 1.0f);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);	//-> 이게 없으면 이전에 그렸던 게 계속 유지되겠지?

	// Scene을 통해 Rendering 될 수 있게
	SCENEMANAGER->Render();
	SwapChain->Present(0, 0);	//스왑체인 표시 옵션 정수 SyncInterval, DXGI_PRESENT
}

//
// 기능: DirectX11 초기화
// 1. D3D11CreateDeviceAndSwapChain()를 이용하여 
//	like	Device(CPU 영역)		: 자원을 생성하는 역할
//	DeviceContext(GPU)				: 그래픽카드 관련해서 일들을 처리하는 영역
//	SwapChain(BackBuffer)			: 영역교체
//
//	--> comInterface으로 되어 있음.
//		특징 
//		1) 직접 메모리를 동적 할당할 수 없음. --> 이중 포인터 타입 내부에서 다렉라이브러리에서 **로 주소만 받는 것
//		내부 데이터는 걔내가 알아서 처리(new 못쓴다는 뜻)
//		2) 지울 때 Release함수를 이용해서 메모리를 해제해야 한다.
//		3)	변수Type에 "I" 라는 문자가 항상 있음. (Interface라는 용어)

//	2. SwapChain으로 backbuffer를 얻어와 CreateRenderTargetView를 생성한다.
//
void MainWindow::Init3D()
{	
	HRESULT hr;
	// D3D11CreateDeviceAndSwapChain()을 이용해	//SwapChain, Device, DeviceContext를 얻음 
	DXGI_MODE_DESC bufferDesc;
	{
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width = this->GetWidth();		// 화면의 폭
		bufferDesc.Height = this->GetHeight();		// 화면의 높이
		bufferDesc.RefreshRate.Numerator = 60;		// 갱신 주기 분자
		bufferDesc.RefreshRate.Denominator = 1;		// 갱신 주기 분모 (60/1, 1초에 60번 Refresh)
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 픽셀포맷
		//UNORM(UINT Normalize : UINT 형 정규화 0~1) 자료형의 값을 R8G8B8A8(8비트->1바이트 씩 4개) 쓰겠다
		//https://nickchooshin.tistory.com/6  이부분은 걍 이거 쓰자. 라고 거의 합의된 상태
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}
	//description 을 사용 할 때 ZeroMemory를 이용해서 0으로 초기화 하여야 한다.
	DXGI_SWAP_CHAIN_DESC swapDesc;
	{
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));	//참고로 ZeroMemory은 내부에서 memset 임. 똑같은거
		swapDesc.BufferDesc = bufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 출력모드
		swapDesc.BufferCount = 1;		// SwapChain 에서 사용할 Back Buffer의 갯수 (1개: 이중 버퍼링)
		swapDesc.SampleDesc.Count = 1;	// 기본형 (샘플링)(안티 앨리어싱)
		swapDesc.SampleDesc.Quality = 0;	// 기본형
		swapDesc.OutputWindow = MAIN->GetWindowHandler();	// 렌더링 결과 출력할 윈도우 핸들
		swapDesc.Windowed = TRUE;			// True: 창모드, False는 창모드 아님
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;		// Swap 효과를 서술하는 구조체
		//swap이 Front, back 왔다갔다 한다고 했지? back에서 front로 바꾼 후 back 데이터를 지운다.는 뜻
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
	//지정된 하드웨어 우선순위를 확인할 변수
	D3D_FEATURE_LEVEL currentFeatureLevel;
	//10167번째 줄
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,	//__in_opt IDXGIAdapter*,	// 비디오카드 지정 변수 (이젠 안쓰는 하드웨어 글카와 다름.)
		D3D_DRIVER_TYPE_HARDWARE, //D3D_DRIVER_TYPE	// HARDWARE가 글카 쓰는 거. F12로 확인
		NULL,	//HMODULE,	// DLL (Thrid Party) ex) OpenGL SGI 같은거 같이쓸때 넣을 모듈
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, //UINT	3D + 2D 혼합형 옵션
		feature_level.data(),	// CONST D3D_FEATURE_LEVEL*	// 지원 가능한 순서 정하기 (글카 구릴수록 아래로: 후퇴전략)
		(UINT)feature_level.size(),	// UINT FeatureLevels // 정의된 갯수
		D3D11_SDK_VERSION,		//UINT SDKVersion // SDK 버전
		&swapDesc,//		DXGI_SWAP_CHAIN_DESC* // swapchain description
		&SwapChain, // IDXGISwapChain** //생성되는 SwapChain  Out 파라미터
		&Device, // ID3D11Device**	// 생성되는 Device  Out 파라미터
		&currentFeatureLevel,//  D3D_FEATURE_LEVEL* // 어떤 게 들어왔는지 확인해보고싶을 때	Out 파라미터 
		&DeviceContext // ID3D11DeviceContext** // 생성되는 DeviceContext	Out 파라미터
	);

	assert(SUCCEEDED(hr));

	// 2. SwapChain으로 backBuffer를 얻어와 CreateRenderTargetView를 생성한다.
	/*
	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));

	//메뉴얼 피셜 2분간 읽고 코딩하세요. (똑같다는 뜻), 이 부분을 CreatebackBuffer() 로 통합
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(BackBuffer);
	
	SCENEMANAGER->GetInstance();
	*/
	CreateBackBuffer();
}