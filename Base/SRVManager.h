#pragma once

/// <summary>
/// Image Data�� �����ϴ� Class
/// ���� ��� Enemy�� ���� ������ ������ų �� 
///	Image�����ʹ� ����, �޸𸮿� ��� ������ų �ʿ�� ����. 
/// </summary>
#define SRVMANAGER SRVManager::GetInstance()
class SRVManager
{
private:
	struct SpriteResource
	{
		ID3D11ShaderResourceView* _SRV;	// Image --> DirectX11 �� ��ȯ
		Vector2 _spriteSize;
	};
	map<wstring, SpriteResource*> _spriteResources;	// Image������ ��� ��
public:
	static SRVManager* GetInstance()
	{
		static SRVManager singleTon; // ������ �ι������� �ʴ� ����
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

