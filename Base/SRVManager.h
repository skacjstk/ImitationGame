#pragma once

/// <summary>
/// Image Data를 관리하는 Class
/// 예를 들어 Enemy인 경우는 여러개 생성시킬 때 
///	Image데이터는 공통, 메모리에 계속 생성시킬 필요는 없음. 
/// </summary>
#define SRVMANAGER SRVManager::GetInstance()
class SRVManager
{
private:
	struct SpriteResource
	{
		ID3D11ShaderResourceView* _SRV;	// Image --> DirectX11 로 변환
		Vector2 _spriteSize;
	};
	map<wstring, SpriteResource*> _spriteResources;	// Image파일을 담는 맵
public:
	static SRVManager* GetInstance()
	{
		static SRVManager singleTon; // 생성이 두번되지는 않는 정적
		return &singleTon;
	}
public:
	SRVManager();
	~SRVManager();
public:	// Setter
	ID3D11ShaderResourceView* CreateShaderResourceView(wstring strFilePath);
	ID3D11ShaderResourceView* FindShaderResourceView(wstring strFileName);
	Vector2 FindSpriteSize(wstring strFileName);
};

