#include "ImitationGame/framework.h"
#include "./Render/TextureShader.h"
#include "Texture.h"

Texture::Texture(wstring strImage, wstring strShader)
{
	_Shader = InputShader(strShader);
	Vector2 size = ConstructorCommonWork(strImage);	// strImageFile ��� ���� ����
	_imageSize = size;
	CreateVertexBuffer(0.0f, 0.0f, size.x, size.y);	
}

Texture::Texture(wstring strImage, wstring strShader, int startX, int startY, int endX, int endY)
{
	_Shader = InputShader(strShader);
	Vector2 size = ConstructorCommonWork(strImage);	// strImageFile ��� ���� ����
	_imageSize = size;
	CreateVertexBuffer((float)startX, (float)startY, (float)endX, (float)endY);
}

/// <summary>
/// ��� �����ڿ��� �������� �����ϴ�, SRV�� �������� �Լ�  (�̿ϼ�)
/// </summary>
Vector2 Texture::ConstructorCommonWork(wstring strImage)
{
//	_strImageFile = IMAGE_FOLDER + strImage;
	_strImageFile = strImage;
	_SRV = SRVMANAGER->CreateShaderResourceView(_strImageFile);
	return SRVMANAGER->FindSpriteSize(_strImageFile);
}

Texture::~Texture()
{
	SAFE_RELEASE(this->_vertexBuffer);
	SAFE_DELETE(_Shader);
}
/// <summary>
/// 1. Matrix  Shader ( W, V, P )
/// 2. Shader ���ٰ� SRV�� 
/// </summary>
void Texture::Update(Matrix V, Matrix P)
{
	// GPU�� vertex������ Update
	// MAP -> UNMAP���̿��� ����

//	D3D11_MAPPED_SUBRESOURCE  subResource;
//	DeviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
//	{
//		memcpy(subResource.pData, vertices, sizeof(Vertex) * m_cvLines.size());
//	}
//	DeviceContext->Unmap(_vertexBuffer, 0);
	// �ڱ� �ڽſ� ���� Matrix
	Matrix W, T, S, R, Pv;
	float gap = PI / 180.0f;
	D3DXMatrixTranslation(&T, _position.x, _position.y, 0.0f);
	D3DXMatrixScaling(&S, _scale.x*_spriteSize.x,
		_scale.y* _spriteSize.y, 0.0f);		// �������� 0 �� �ƴϾ�� ��. 
	D3DXMatrixRotationYawPitchRoll(&R, _rotation.y * gap,
		_rotation.x * gap,
		_rotation.z * gap);	//yaw pitch roll ��
	D3DXMatrixTranslation(&Pv, _pivot.x, _pivot.y, _pivot.z);
	D3DXMatrixInverse(&Pv, NULL, &Pv);
	W = S * Pv * R  * T;

	// update �ֱ⸶�� Shader �� SRV ���� 
	_SRV = SRVMANAGER->FindShaderResourceView(_strImageFile);
	this->_imageSize = SRVMANAGER->FindSpriteSize(_strImageFile);

	//Update Block ����
	UpdateBlock();	// offset, size ���� 

	// Shader �� SRV ����
	_Shader->SetShaderResourceView(0, _SRV);	// 0�� index�� _SRV �ֱ� 
	_Shader->Update(W, V, P);
}

void Texture::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// IA�ܰ�  : Device
	DeviceContext->IASetVertexBuffers(0,   // slot
		1,   // buffer�� ����
		&this->_vertexBuffer,
		&stride,
		&offset);

	DeviceContext->IASetInputLayout(this->_Shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS ~ OM �ܰ�
	_Shader->Draw(6, 0);
}

/// <summary>
// Offset, OffsetSize �� �̿��Ͽ�
// Uv ���� ����	--> Animation, AnimationClip���� �� �Ѱ���
// Texture�� �̿��� �� �ְԲ� 
/// </summary>
void Texture::UpdateBlock()
{
	Vertex vertices[6];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);	// 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);	// 2

	vertices[3].Position = Vector3(-0.5f, -0.5f, 0.0f);	// 0
	vertices[4].Position = Vector3(+0.5f, +0.5f, 0.0f);	// 2
	vertices[5].Position = Vector3(+0.5f, -0.5f, 0.0f);	// 3

	// ������� ( NDC ��ǥ�� ��ȯ. 800 -> 1.0f ���� �ٲ�µ�, 20/800 = 0.0025 �̷�������
	float startX = _spriteOffset.x / _imageSize.x;
	float startY = _spriteOffset.y / _imageSize.y;
	float endX = (_spriteOffset.x + _spriteSize.x) / _imageSize.x;
	float endY = (_spriteOffset.y + _spriteSize.y) / _imageSize.y;

	/*		sxsy	 |		exsy
	 *				 |
	 *		 -----------------
	 *				 |
	 *		sxey	 |		exey
	*/
	vertices[0].Uv = Vector2(startX, endY);		  // 0
	vertices[1].Uv = Vector2(startX, startY);	  // 1
	vertices[2].Uv = Vector2(endX, startY);		  // 2

	vertices[3].Uv = Vector2(startX, endY);		  // 0
	vertices[4].Uv = Vector2(endX, startY);		  // 2
	vertices[5].Uv = Vector2(endX, endY);		  // 3

	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(Vertex) * 6);
	}
	DeviceContext->Unmap(_vertexBuffer, 0);
}

void Texture::CreateShaderResourceView(wstring strFile)
{
	SRVMANAGER->CreateShaderResourceView(strFile);
}
void Texture::CreateMultiShaderResourceView(wstring strFile)
{
	SRVMANAGER->CreateShaderResourceView(strFile);
	// ���߿� Shader�� �����ִ� function 
}
void Texture::UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3)
{
	_Shader->UpdateColorBuffer(color, path, Time, Time2, Time3);
}
void Texture::UpdateColorBuffer(int path)
{
	Color color(0.0f, 0.0f, 0.0f, 0.0f);
	_Shader->UpdateColorBuffer(color, path, 0.0f, 0.0f, 0.0f);
}
void Texture::UpdateColorBuffer(Color color, int path)
{
	_Shader->UpdateColorBuffer(color, path, 0.0f, 0.0f, 0.0f);
}
void Texture::UpdateColorBuffer(Color color, int path, float Time)
{
	_Shader->UpdateColorBuffer(color, path, Time, 0.0f, 0.0f);
}

/// <summary>
/// Position, UV�� Create
/// </summary>
void Texture::CreateVertexBuffer(float startX, float startY, float endX, float endY)
{
	Vertex vertices[6];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);	// 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);	// 2

	vertices[3].Position = Vector3(-0.5f, -0.5f, 0.0f);	// 0
	vertices[4].Position = Vector3(+0.5f, +0.5f, 0.0f);	// 2
	vertices[5].Position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
	// �̹��� ������ 0 1 3 3 1 2 �� �ٲپ���. �ٵ� �� �װŽȾ� 

	Vector2 size = SRVMANAGER->FindSpriteSize(_strImageFile);

	this->_spriteOffset = Vector2(startX, startY);
	this->_spriteSize = Vector2(endX - startX, endY - startY);
	// ������� ( NDC ��ǥ�� ��ȯ. 800 -> 1.0f ���� �ٲ�µ�, 20/800 = 0.0025 �̷�������

	startX = startX / size.x;
	startY = startY / size.y;
	endX = endX / size.x;
	endY = endY / size.y;

	/*		sxsy	 |		exsy
	 *				 |
	 *		 -----------------
	 *				 |
	 *		sxey	 |		exey
	*/
	vertices[0].Uv = Vector2(startX, endY);		  // 0
	vertices[1].Uv = Vector2(startX, startY);	  // 1
	vertices[2].Uv = Vector2(endX, startY);		  // 2

	vertices[3].Uv = Vector2(startX, endY);		  // 0
	vertices[4].Uv = Vector2(endX, startY);		  // 2
	vertices[5].Uv = Vector2(endX, endY);		  // 3
	// Uv ��ǥ��� ������ ��ǥ�踦 0~1�� ǥ���� �� 
	// 
	// 2. CPU�� �ڿ��� GPU�� IA �ܰ迡 �Ѱ��ֱ� ���� ��ȯ �۾��� �����Ѵ�.
	//
	// DX_Line, DX_Triangle���� �޸�, 
	D3D11_BUFFER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;	//GPU �����Ͱ� �Ѿ ��, �����͸� ���� ���� (DYNAMIC - ����)
		desc.ByteWidth = sizeof(Vertex) * 6;	//_vertices�� 4���� ������ ���� ����ü�� �� 
											// Class�� �Ǳ�� ������, ByteWidth ����� ��ƴ�. (������ ��������?)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �� desc�� Vertex Buffer
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;	// ������ �����ϱ� ���� ���� ���� ������
	//	D3D11_BIND_VERTEX_BUFFER = 0x1L,	// ��� ���� ���
	//	D3D11_BIND_INDEX_BUFFER = 0x2L,		// 6�� ������ 4���� ���̰�, �ﰢ���� �����ϴ� Index 3D���� ü�� Ŀ
	//	D3D11_BIND_CONSTANT_BUFFER = 0x4L,	// ������ ������� Matrix, float4, 
	//	D3D11_BIND_SHADER_RESOURCE = 0x8L,	// Texture ( bmp, png, �� ���� Resource )
	}
	D3D11_SUBRESOURCE_DATA data;
	{
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;	//ù��° ������ ������, 
	}
	// �ڿ��� Device�κ��� ����� ( SwapChain, Device, DeviceContext, RTV) 
	// Device�� CPU�� ������ ,DC�� GPU�� ������.
	// �ᱹ CreateBuffer�Ҷ� �ڿ��� Device�κ��� �����.
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->_vertexBuffer);
	// �� Com �������̽� buffer�� data, desc ������ ���� ���� ������ ������.
	assert(SUCCEEDED(hr));
}
TextureShader* Texture::InputShader(wstring strShader)
{
	return new TextureShader(strShader);
}