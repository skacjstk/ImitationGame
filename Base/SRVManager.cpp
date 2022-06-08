#include "ImitationGame/framework.h"
#include <io.h>
#include "ImitationGame/MainWindow.h"
#include "SRVManager.h"

SRVManager::SRVManager()
{
}

SRVManager::~SRVManager()
{
	for (auto v : _spriteResources)
	{
		SAFE_RELEASE(v.second->_SRV);
		delete v.second;
	}
	_spriteResources.clear();
}

ID3D11ShaderResourceView* SRVManager::CreateShaderResourceView(wstring strFilePath)
{
	wstring strFileName = strFilePath;
	// wstring to string (assign 안씀)
	if (_waccess(strFileName.c_str(), 00) != 0)
	{
		strFileName = L"SRVManager::CreateShaderResourceView file Not Found" + strFileName;
		MessageBoxW(MAIN->GetWindowHandler(),
			strFileName.c_str(),
			L"FILE ERROR",
			MB_OK);
		return nullptr;	// 파일 없음.
	}

	// 기존에 map에 데이터가 들어가 있는지 검사 (중복 방지)
	if (_spriteResources.find(strFileName) != _spriteResources.end())
		return _spriteResources.find(strFileName)->second->_SRV;

	// ShaderResourceView SRV를 생성
	// 생성 Style 이 3가지 정도 있음
	// 1. Memory(dds)  dds는 다렉에서 만들어진 파일 포멧
	// 2. stream(dds)
	// 3. file(*.png) 
	SpriteResource* spriteResource = new SpriteResource();

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		Device,						//ID3D11Device*              
		strFileName.c_str(),		//LPCWSTR                    
		NULL,						//D3DX11_IMAGE_LOAD_INFO     
		NULL,						//ID3DX11ThreadPump*         
		&spriteResource->_SRV,		//ID3D11ShaderResourceView** 
		NULL);						//HRESULT*                   
	assert(SUCCEEDED(hr));

	// 데이터를 집어넣기 전, ImageFile의 width, height 를 얻으려면 
	// 별도의 함수로 얻어와야 함
	D3DX11_IMAGE_INFO ImageInfo;
	hr = D3DX11GetImageInfoFromFile(strFileName.c_str(), NULL, &ImageInfo, NULL);
	assert(SUCCEEDED(hr));

	spriteResource->_spriteSize =
		Vector2((float)ImageInfo.Width, (float)ImageInfo.Height);

	_spriteResources.insert(make_pair(strFileName, spriteResource));
	return spriteResource->_SRV;
}
/// <summary>
/// 레거시
/// </summary>
ID3D11ShaderResourceView* SRVManager::FindShaderResourceView(wstring strFileName)
{
	if (_spriteResources.find(strFileName) != _spriteResources.end())
	{
		return _spriteResources.find(strFileName)->second->_SRV;
	}
	return nullptr;
}

Vector2 SRVManager::FindSpriteSize(wstring strFileName)
{
	if (_spriteResources.find(strFileName) != _spriteResources.end())
		return _spriteResources.find(strFileName)->second->_spriteSize;
	else
		return Vector2(0.0f, 0.0f);
}
