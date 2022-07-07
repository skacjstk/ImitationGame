#include "ImitationGame/framework.h"
#include "./Render/TextureShader.h"
#include "Texture.h"

Texture::Texture(wstring strImage, wstring strShader)
{
	_Shader = InputShader(strShader);
	Vector2 size = ConstructorCommonWork(strImage);	// strImageFile 경로 삽입 시점
	_imageSize = size;
	CreateVertexBuffer(0.0f, 0.0f, size.x, size.y);	
}

Texture::Texture(wstring strImage, wstring strShader, int startX, int startY, int endX, int endY)
{
	_Shader = InputShader(strShader);
	Vector2 size = ConstructorCommonWork(strImage);	// strImageFile 경로 삽입 시점
	_imageSize = size;
	CreateVertexBuffer((float)startX, (float)startY, (float)endX, (float)endY);
}

/// <summary>
/// 모든 생성자에서 공통으로 수행하는, SRV를 가져오는 함수  (미완성)
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
/// 2. Shader 에다가 SRV를 
/// </summary>
void Texture::Update(Matrix V, Matrix P)
{
	// GPU에 vertex내용을 Update
	// MAP -> UNMAP사이에서 수정

//	D3D11_MAPPED_SUBRESOURCE  subResource;
//	DeviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
//	{
//		memcpy(subResource.pData, vertices, sizeof(Vertex) * m_cvLines.size());
//	}
//	DeviceContext->Unmap(_vertexBuffer, 0);
	// 자기 자신에 대한 Matrix
	Matrix W, T, S, R, Pv;
	float gap = PI / 180.0f;
	D3DXMatrixTranslation(&T, _position.x, _position.y, 0.0f);
	D3DXMatrixScaling(&S, _scale.x*_spriteSize.x,
		_scale.y* _spriteSize.y, 0.0f);		// 스케일은 0 이 아니어야 함. 
	D3DXMatrixRotationYawPitchRoll(&R, _rotation.y * gap,
		_rotation.x * gap,
		_rotation.z * gap);	//yaw pitch roll 순
	D3DXMatrixTranslation(&Pv, _pivot.x, _pivot.y, _pivot.z);
	D3DXMatrixInverse(&Pv, NULL, &Pv);
	W = S * Pv * R  * T;

	// update 주기마다 Shader 에 SRV 세팅 
	_SRV = SRVMANAGER->FindShaderResourceView(_strImageFile);
	this->_imageSize = SRVMANAGER->FindSpriteSize(_strImageFile);

	//Update Block 먼저
	UpdateBlock();	// offset, size 변경 

	// Shader 에 SRV 세팅
	_Shader->SetShaderResourceView(0, _SRV);	// 0번 index에 _SRV 넣기 
	_Shader->Update(W, V, P);
}

void Texture::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// IA단계  : Device
	DeviceContext->IASetVertexBuffers(0,   // slot
		1,   // buffer의 갯수
		&this->_vertexBuffer,
		&stride,
		&offset);

	DeviceContext->IASetInputLayout(this->_Shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS ~ OM 단계
	_Shader->Draw(6, 0);
}

/// <summary>
// Offset, OffsetSize 를 이용하여
// Uv 값을 변경	--> Animation, AnimationClip에서 단 한개의
// Texture를 이용할 수 있게끔 
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

	// 비율계산 ( NDC 좌표로 변환. 800 -> 1.0f 으로 바뀌는데, 20/800 = 0.0025 이런식으로
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
	// 나중에 Shader에 보내주는 function 
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
/// Position, UV를 Create
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
	// 이번엔 순서를 0 1 3 3 1 2 로 바꾸었다. 근데 난 그거싫어 

	Vector2 size = SRVMANAGER->FindSpriteSize(_strImageFile);

	this->_spriteOffset = Vector2(startX, startY);
	this->_spriteSize = Vector2(endX - startX, endY - startY);
	// 비율계산 ( NDC 좌표로 변환. 800 -> 1.0f 으로 바뀌는데, 20/800 = 0.0025 이런식으로

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
	// Uv 좌표계는 윈도우 좌표계를 0~1로 표현한 값 
	// 
	// 2. CPU의 자원을 GPU중 IA 단계에 넘겨주기 위한 변환 작업을 수행한다.
	//
	// DX_Line, DX_Triangle과는 달리, 
	D3D11_BUFFER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;	//GPU 데이터가 넘어간 후, 데이터를 수정 여부 (DYNAMIC - 가능)
		desc.ByteWidth = sizeof(Vertex) * 6;	//_vertices를 4개만 썻었지 보통 구조체로 씀 
											// Class도 되기는 되지만, ByteWidth 계산이 어렵다. (이제는 쉬울지도?)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 이 desc는 Vertex Buffer
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;	// 데이터 수정하기 위한 접근 가능 지정자
	//	D3D11_BIND_VERTEX_BUFFER = 0x1L,	// 모든 정점 사용
	//	D3D11_BIND_INDEX_BUFFER = 0x2L,		// 6개 정점을 4개로 줄이고, 삼각형을 연결하는 Index 3D에서 체감 커
	//	D3D11_BIND_CONSTANT_BUFFER = 0x4L,	// 정점과 관계없는 Matrix, float4, 
	//	D3D11_BIND_SHADER_RESOURCE = 0x8L,	// Texture ( bmp, png, 에 대한 Resource )
	}
	D3D11_SUBRESOURCE_DATA data;
	{
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;	//첫번째 번지수 보내기, 
	}
	// 자원은 Device로부터 만든다 ( SwapChain, Device, DeviceContext, RTV) 
	// Device는 CPU에 가깝고 ,DC는 GPU에 가깝다.
	// 결국 CreateBuffer할때 자원은 Device로부터 만든다.
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->_vertexBuffer);
	// 이 Com 인터페이스 buffer에 data, desc 절차에 의해 여러 정보를 복사함.
	assert(SUCCEEDED(hr));
}
TextureShader* Texture::InputShader(wstring strShader)
{
	return new TextureShader(strShader);
}