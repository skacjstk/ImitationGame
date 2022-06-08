// ImitationGame.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "ImitationGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:

CMouse* Mouse = NULL;	// 실패할 수 있음

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MAIN->CreateInstance(hInstance, 800, 600);
    // IMGui Setting

    // Mouse Setting
    Mouse = new CMouse(MAIN->GetWindowHandler());
    MSG msg;

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
            MAIN->Update();
            MAIN->Render();
        }
    }

   
    return (int) msg.wParam;
}