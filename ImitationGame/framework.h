// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

// 시스템 환경변수에 DH 선언 
// DH = C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
// 시스템 환경변수에 DL 선언 (library)
// x64도 있음.
// DL = C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86
// 입력 후 시스템 재부팅 권장 ( 유저 환경변수에 넣었을 경우, 로그아웃만)
// 포함 디렉터리에 $(DH); 추가
// 
// 라이브러리 디렉터리에 $(DL); 추가

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// #define _HAS_STD_BYTE 0		// C++17 std::byte를 활성화 해제 https://blog.cxx.kr/archives/390
// Windows 헤더 파일
#include <windows.h>
#include <shellapi.h>		// Drag, explorer 등등
#include <commdlg.h>		// File Select, 기타 등등 Dialog로 띄워지는 것들
// C 런타임 헤더 파일입니다.
#include <assert.h>		//HRESULT 를 가지고 유효성 검사를 할 수 있음.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
// C++ Header
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <functional>
#include <mutex>
// using std 
using std::string;
using std::wstring;
using std::vector;
using std::map;
using std::bitset;
using std::thread;

// using std Function
using std::bind;
using std::to_wstring;
using std::to_string;

//DX Library
#include <d3d11.h>	
#include <D3DX11.h>
#include <D3DX10.h>	
#include <D3DX10math.h>
// #include <d3dx11effect.h>	// 32bit 용(그래서 x86을 썻었음)  --> 64bit로 변경 예정
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "./_Libraries/Effects11d.lib")
// 
// DirectX 2D font
#include  <d2d1_2.h>
#include  <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

//SAFE_DELETE Marco
#define SAFE_DELETE(p) { if(p) {delete (p); (p) = NULL ;} }
#define SAFE_DELETE_ARRAY(p) {  if(p) {delete[] (p); (p) = NULL ;} }
#define SAFE_RELEASE(p) { if(p) {(p)->Release(); (p) = NULL ;} }
#define PI 3.141592f
#define SHADER_FOLDER	L"../Shader/"
#define IMAGE_FOLDER	L"../Resources/Image/"
#define AUDIO_FOLDER	L"../Resources/AudioClip/"

//typedef		
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;		//이름 맞춰놓기용
typedef D3DXMATRIX	Matrix;
typedef D3DXCOLOR	Color;

// IMGUI 1.6 x86
// #include "./_Libraries/imgui.h"
// #include "./_Libraries/imguiDx11.h"
// #pragma comment(lib, "./_Libraries/ImGui.lib")
#include "./ImGUI/imgui.h"
#include "./ImGUI/imgui_impl_win32.h"
#include "./ImGUI/imgui_impl_dx11.h"
/*
#if _WIN64
#pragma comment(lib, "./ImGUI/ImGui64.lib")
#else
#pragma comment(lib, "./ImGUI/ImGui.lib")
#endif
*/
// Sqlite3
// #include "Sqlite/sqlite3.h"
//User 정의
#include "Base/Scene.h"
#include "Base/SceneManager.h"
#include "MainWindow.h"
#include "System/GameTime.h"
#include "System/Keyboard.h"
#include "System/Mouse.h"
#include "./Base/SRVManager.h"
#include "./Render/Texture.h"
#include "./Base/Camera.h"
#include "./Base/ObjectManager.h"
#include "./Render/AnimationClip.h"
#include "./Render/Animation.h"
//#include "./Base/Terrain.h"
#include "./Base/GameObject.h"
#include "./Base/GameActor.h"
#include "./Base/UIObject.h"
#include "./Object/Button.h"
//#include "./Object/Tile.h"
#include "./Render/DirectWrite.h"

//DX Cominterface 및 Global
//MainWindow.cpp 에 정의되어 있음.
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern IDXGISwapChain* SwapChain;
extern ID3D11RenderTargetView* RTV;		//실제로 보는 영역
extern CMouse* Mouse;
extern bool g_bVisibleCollisionCheck;
extern const float G;
extern float WSCALEX;	// 오프셋 배율 계산용
extern float WSCALEY;	// 오프셋 배율 계산용