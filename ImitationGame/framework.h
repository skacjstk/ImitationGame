// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <shellapi.h>		// Drag, explorer, HDROP
#include <commdlg.h>		// File Select, 기타 등등 Dialog로 띄워지는 것들
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <assert.h>		//HRESULT 를 가지고 유효성 검사를 할 수 있음.
#include <stdio.h>

// C++ Header
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <functional>
#include <mutex>
using namespace std;

// DX Library
#include <d3d11.h>	
#include <D3DX11.h>
#include <D3DX10.h>	
#include <D3DX10math.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")

// IMGUI	// 폴더 명 단위
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

/*
#if _WIN64
#pragma comment(lib, "ImGUI/ImGui64.lib")
#else
#pragma comment(lib, "ImGUI/ImGui.lib")
#endif
*/
//SAFE_DELETE Marco
#define SAFE_DELETE(p) { if(p) {delete (p); (p) = NULL ;} }
#define SAFE_DELETE_ARRAY(p) {  if(p) {delete[] (p); (p) = NULL ;} }
#define SAFE_RELEASE(p) { if(p) {(p)->Release(); (p) = NULL ;} }
#define PI 3.141592f

//typedef		
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;		//이름 맞춰놓기용
typedef D3DXMATRIX	Matrix;
typedef D3DXCOLOR	Color;

extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern IDXGISwapChain* SwapChain;
extern ID3D11RenderTargetView* RTV;		//실제로 보는 영역
// extern CMouse* Mouse;