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
	// wstring to string (assign �Ⱦ�)
	if (_waccess(strFileName.c_str(), 00) != 0)
	{
		strFileName = L"SRVManager::CreateShaderResourceView file Not Found" + strFileName;
		MessageBoxW(MAIN->GetWindowHandler(),
			strFileName.c_str(),
			L"FILE ERROR",
			MB_OK);
		return nullptr;	// ���� ����.
	}

	// ������ map�� �����Ͱ� �� �ִ��� �˻� (�ߺ� ����)
	if (_spriteResources.find(strFileName) != _spriteResources.end())
		return _spriteResources.find(strFileName)->second->_SRV;

	// ShaderResourceView SRV�� ����
	// ���� Style �� 3���� ���� ����
	// 1. Memory(dds)  dds�� �ٷ����� ������� ���� ����
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

	// �����͸� ����ֱ� ��, ImageFile�� width, height �� �������� 
	// ������ �Լ��� ���;� ��
	D3DX11_IMAGE_INFO ImageInfo;
	hr = D3DX11GetImageInfoFromFile(strFileName.c_str(), NULL, &ImageInfo, NULL);
	assert(SUCCEEDED(hr));

	spriteResource->_spriteSize =
		Vector2((float)ImageInfo.Width, (float)ImageInfo.Height);

	_spriteResources.insert(make_pair(strFileName, spriteResource));
	return spriteResource->_SRV;
}
/// <summary>
/// ���Ž�
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
