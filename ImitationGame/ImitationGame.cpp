// ImitationGame.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "ImitationGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:

CMouse* Mouse = NULL;	// 실패할 수 있음
void LoadFont();
//Window Create
//1. 윈도우 클래스를 정의 레지스트리에 등록
//2. CreateWindow() 를 이용해 윈도우 생성 
//		hInstance는 인스턴스. --> C++에 클래스로 찍어낸 실제 오브젝트
//		HANDLE --> 포인터X, 4바이트 식별자, h가 핸들이라고
//		Window  OS에서 입력으로 hInstance 생성후 전달.
//	*HINSTANCE hPrevInstance
//		실행 -> Kill -> 프로그램 재실행 용. MFC는 4번정도 가능해.
//	*LPWSTR lpCmdLine : char* 와 같음.
//		long pointer,  즉 C에서 *WCHAR 와 같음.
//	*int nCmdShow : 현재 프로그램이 보이게 될 때의 show (보이는 형태)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MAIN->CreateInstance(hInstance, 1920, 1080);
    // IMGui Setting
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplWin32_Init(MAIN->GetWindowHandler());
    ImGui_ImplDX11_Init(Device, DeviceContext);	// Create() 대신
    LoadFont();	// TTF True Type Font
    ImGui::StyleColorsDark();	// 구버전꺼 그대로 
    // Mouse Setting
    Mouse = new CMouse(MAIN->GetWindowHandler());

    // DirectX 2D font
    DirectWrite::Create();
    MSG msg;

    //GetMessage, PeekMessage, PumpMessage가 있음.
    //	wait	  No wait      Message큐에서 강제로 뽑아 쓸 때
    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

            if (msg.message == WM_QUIT)
                break;
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            TIMEMANAGER->Update(144.0f);//144프레임: 6.94, 60프레임: 16 근데 여기 입력값이 프레임 단위 제한임.		
       //     TIMEMANAGER->Update();    // 프레임 무제한 업데이트: 주로 최적화 테스트용( 둘중 하나만 쓸 것 )
            // Mouse position
            Mouse->Update();
            MAIN->Update();
            MAIN->Render();
        }
    }

   
    return (int) msg.wParam;
}

// TTF File Load <fstream> 필요 
void LoadFont()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = nullptr;
    std::string font_file = "../TTF/malgunbd.ttf";
    //std::string font_file = "imgui/fonts/D2Coding.ttf";

    std::ifstream ifile;
    ifile.open(font_file);
    if (ifile) {
        std::cout << "file exists\n";
        font = io.Fonts->AddFontFromFileTTF(font_file.c_str(),
            16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    }
    else {
        std::cout << "file doesn't exist\n";
        font = io.Fonts->AddFontDefault();
    }
    IM_ASSERT(font != NULL);
}

