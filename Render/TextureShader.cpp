#include "ImitationGame/framework.h"
#include <io.h>
#include "TextureShader.h"

TextureShader::TextureShader(wstring shaderFile)
{
	//	wstring File = SHADER_FOLDER + shaderFile;
	wstring File = shaderFile;

	// 초기화
	for (UINT i = 0; i < _countof(_SRV); ++i)
	{
		_SRV[i] = nullptr;
	}
	if (CheckFile(File))
	{
		CreateShader(File);
		CreateInputLayout();
		CreateMatrixBuffer();
		CreateColorBuffer();

		CreateSampleState();
		CreateBlendState();
		CreateRasterState();
	}
}

TextureShader::~TextureShader()
{
	SAFE_RELEASE(_vertexShader);			// c++정점->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_pixelShader);				// Shader Pixel
	SAFE_RELEASE(_vsBlob);					// HLSL->Binary변환
	SAFE_RELEASE(_psBlob);					// HLSL->Binary변환
	SAFE_RELEASE(_inputLayout);				// IA
	SAFE_RELEASE(_matrixBuffer);
	SAFE_RELEASE(_colorBuffer);
	SAFE_RELEASE(_sampleState);
	SAFE_RELEASE(_rasterState);
	SAFE_RELEASE(_blendState);
	SAFE_RELEASE(_blendStateOff);
	//SRV는 SRV의 원본 SRVManager 쪽에서 지울 거라 없어도 된다.
}
// ColorShader 와 똑같음.
void TextureShader::Update(Matrix W, Matrix V, Matrix P)
{
	TRANSFORM_DATA temp;
	// 순수행렬
	// Sx	?	?	Tx					<---				// Sx	?	?	0
	// ?	Sy	?	Ty										// ?	Sy	?	0
	// ?	?	Sz	Tz		// ? 는 rotate 자리 			   	// ?	?	Sz	0	
	// 0	0	0	W	//동차 							   	// Tx	Ty	Tz	W	
	// 이게 original Matrix Format							// DirectX 전치행렬
	// 이래서 변환을 해줘야 한다
	D3DXMatrixTranspose(&temp.Projection, &P);
	D3DXMatrixTranspose(&temp.View, &V);
	D3DXMatrixTranspose(&temp.World, &W);

	// 상수 버퍼에 써주는 역할을 하면 된다.
	// MAP -> UNMAP 사이에서 수정
	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, &temp, sizeof(TRANSFORM_DATA));
	}
	DeviceContext->Unmap(_matrixBuffer, 0);
}
// ColorShader 기반 (RS 추가)
void TextureShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// VS단계
	DeviceContext->VSSetShader(_vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers 과 같은 놈

	// RS: 단계 
	DeviceContext->RSSetState(_rasterState);
	// PS
	DeviceContext->PSSetShader(_pixelShader, nullptr, 0);	// 원래 0이었는데 nullptr로 바뀐건 무슨 의미일까
	DeviceContext->PSSetSamplers(0, 1, &_sampleState);	// Line 데이터 ? 추가 

	if (_SRV[1])
		DeviceContext->PSSetShaderResources(0, 2, _SRV); // 2개를 보내
	else
		DeviceContext->PSSetShaderResources(0, 1, _SRV); // 1개를 보내	( 0429 Get 실수 했었음 ) 

	DeviceContext->PSSetConstantBuffers(1, 1, &_colorBuffer);	// VS 에서 0번은 썻으니까, 1번지에 넣겠다 이런거 

	// PipeLine 작동 (OM)
	DeviceContext->OMSetBlendState(_blendState, nullptr, 0xffffffff);

	DeviceContext->Draw(VertexCount, StartVertexLocation);
}

void TextureShader::DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation)
{
}

void TextureShader::UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3)
{
	COLOR_DATA temp;
	temp.color = color;
	temp.path = path;
	temp.time = Time;
	temp.dummy1 = Time2;
	temp.dummy2 = Time3;

	// Map 과 Unmap 사이에서 GPU에 Vertex 내용을 Update
	// 상수 버퍼에 써주는 역할을 하면 된다.
	// MAP -> UNMAP 사이에서 수정
	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, &temp, sizeof(COLOR_DATA));
	}
	DeviceContext->Unmap(_colorBuffer, 0);
}

///
/// VS, PS에 관련 셰이더 생성
/// Blob(파일로 컴파일) --> Shader 생성 기존대비 다를게 없음. ColorShader 에서 가져옴
///
void TextureShader::CreateShader(wstring shaderFile)
{
	HRESULT  hr;
	HRESULT  result;
	ID3D10Blob* pErrorMsgs = NULL;

	// HLSL( VS )을 컴파일후 Blob에 저장
	// Binary Large Object
	// Shader을 생성하기 위해 먼저 Blob생성해야 한다
	{
		hr = D3DX11CompileFromFile(
			shaderFile.c_str(),   // Source File
			NULL,                      // MACRO
			NULL,                      // include
			"VS",                      // Effect.hlsk에 정의된 function명
			"vs_5_0",                  // hlsl에 사용되는 version명
			0,                         // Flag1
			0,                         // Flag2
			NULL,                      // ThredPump 사용안함
			&this->_vsBlob,          // 컴파일후 --> Blob에 저장
			&pErrorMsgs,
			&result
		);

		if (pErrorMsgs)
			OutputErrorMessage(pErrorMsgs);


		assert(SUCCEEDED(hr));
	}

	pErrorMsgs = NULL;

	// HLSL( PS )을 컴파일후 Blob에 저장
	// Binary Large Object
	// Shader을 생성하기 위해 먼저 Blob생성해야 한다
	{
		hr = D3DX11CompileFromFile(
			shaderFile.c_str(),   // Source File
			NULL,                      // MACRO
			NULL,                      // include
			"PS",                      // Effect.hlsk에 정의된 function명
			"ps_5_0",                  // hlsl에 사용되는 version명
			0,                         // Flag1
			0,                         // Flag2
			NULL,                      // ThredPump 사용안함
			&this->_psBlob,          // 컴파일후 --> Blob에 저장
			&pErrorMsgs,
			&result
		);

		if (pErrorMsgs)
			OutputErrorMessage(pErrorMsgs);


		assert(SUCCEEDED(hr));
	}

	// 만들어진 Blob을 통해 Shader생성
	// VertexShader생성
	{
		hr = Device->CreateVertexShader(_vsBlob->GetBufferPointer(),
			_vsBlob->GetBufferSize(),
			NULL,
			&this->_vertexShader);
		assert(SUCCEEDED(hr));
	}

	// 만들어진 Blob을 통해 Shader생성
	// PixelShader생성
	{
		hr = Device->CreatePixelShader(_psBlob->GetBufferPointer(),
			_psBlob->GetBufferSize(),
			NULL,
			&this->_pixelShader);
		assert(SUCCEEDED(hr));
	}

	// Shader 프로그램에 정점,상수등을 보내기위해
}

/// <summary>
/// 생성된 Vertex 의 구성이 어떻게 되어있냐를 알려줌.
/// 여러개의 shader를 만들어 사용해야 할 수도 있음.
/// Uv와 Color는 다른데, 그래서 InputLayout 여러개 -> 어떤건 Color인데 어떤건 TEXCOORD0(uv) 그뜻임.
/// Uv 가 TEXCOORD1 하나 더 생기면 inputLayout이 3개가 되지? 그런 거임.
/// </summary>
void TextureShader::CreateInputLayout()
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layout[2];

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Vector3 float x, y, z
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;  // 텍스쳐에서 한 실수 고치기, TEXCOORD 는 Vector2
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = 12;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;
	// 2개 이상일 경우 변경 
	hr = Device->CreateInputLayout(layout, 2, _vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		&this->_inputLayout);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateMatrixBuffer()
{
	// 위에 선언된 데이터를 VertextBuffer에 Setting
	// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
	// 무조건 Desctrpion이 붙는다
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(TRANSFORM_DATA);       // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     정점
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      중복되는 정점을 Index로 구분하여 사용
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   상수Buffer, User가 HLSL에 데이터 보낼때
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile

	}
	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &_matrixBuffer);
	//	printf("%d\n", sizeof(Vertex));
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateColorBuffer()
{	
	// 위에 선언된 데이터를 VertextBuffer에 Setting
// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
// 무조건 Desctrpion이 붙는다
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(COLOR_DATA);       // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     정점
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      중복되는 정점을 Index로 구분하여 사용
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   상수Buffer, User가 HLSL에 데이터 보낼때
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile

	}
	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &_colorBuffer);
	//	printf("%d\n", sizeof(Vertex));
	assert(SUCCEEDED(hr));
}

void TextureShader::OutputErrorMessage(ID3D10Blob* errorMsgs)
{
}

bool TextureShader::CheckFile(wstring shaderFileName)
{
	bool flag = true;
	wstring wstr;
	if (_waccess(shaderFileName.c_str(), 00) != 0)
	{
		flag = false;
		wstr = L"Shader File Not Found >>" + shaderFileName;
		MessageBoxW(NULL, wstr.c_str(), L"Shader Error", MB_OK);
	}
	return flag;
}

void TextureShader::CreateSampleState()
{
	D3D11_SAMPLER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		// 정규화된 좌표 (NDC: normalized device coordinate) 를 넘었을 때 어떻게 할 지 
		// WRAP		: 원본픽셀로 나머지 부분을 채운다
		// CLAMP	: 맨마지막 픽셀로 채운다
		// MIRROR	: 인접한 픽셀로 반전시킨다. 
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;					//	D3D11_TEXTURE_ADDRESS_MODE
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;					//  D3D11_TEXTURE_ADDRESS_MODE
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;					//  D3D11_TEXTURE_ADDRESS_MODE

		desc.BorderColor[0] = 1; // 외곽선에 대한 white 라는 뜻 (현재는)
		desc.BorderColor[1] = 0; // 외곽선에 대한 white 라는 뜻 (현재는)
		desc.BorderColor[2] = 0; // 외곽선에 대한 white 라는 뜻 (현재는)
		desc.BorderColor[3] = 1; // 외곽선에 대한 white 라는 뜻 (현재는)

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// 이전 / 현재 데이터를 비교하는 Flag,
		// 기존 SAMPLE과 4개(BorderColor의 수) 값을 비교하는데, 항상 비교하라 

		// MIN --> 이미지 축소
		// MAG --> 이미지 확대 
		// 이미지를 확대하거나 축소할 때 미리 1/4 크기로 이미지를 구성한다. 
		// POINT : 2개의 pixel이 있다면 하나를 폐기.
		// LINEAR : 2개의 pixel의 평균값 
		desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		desc.MaxAnisotropy = 16;	// 비등방성 필터 
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		//	FLOAT MipLODBias;
	}
	HRESULT hr = Device->CreateSamplerState(&desc, &_sampleState);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateBlendState()
{
	// 알파블랜딩 할꺼냐 말꺼냐 이런거 설정
	D3D11_BLEND_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;	// 뒤에 테두리를 제거하겠다.
		desc.IndependentBlendEnable = true;	// 개별적인 blend를 하겠다. false 일 경우 알파값이 혼합되지 않음.
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;	// 의문: 어쨋든 얘가 원본이라는건가?	얘가 0.9 
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//  얘는 0.9 --> 0.1 로 바뀜 
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;	// operation add 

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	// 지금은 뒤가 살짝 보이게 만들어짐 
	HRESULT hr = Device->CreateBlendState(&desc, &_blendState);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateRasterState()
{
	// 환경에 따라 나중에 desc가 바뀔 수 있어, 
	D3D11_RASTERIZER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;	// 내부를 칠해 말어 : 칠함
		desc.CullMode = D3D11_CULL_NONE;	// 3D 에서 안보이는 부분이 생기는데, 그 부분을 채울지 말지에 대한 것
		// culling: 추스려내다. 3D에서 안보이는 영역을 추스려 그림 그리게 하지 말라는 표현
		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_cull_mode
		// None이 항상 그리는 것
		desc.FrontCounterClockwise = false;
		desc.DepthBias;
		desc.DepthBiasClamp;
		desc.SlopeScaledDepthBias;
		desc.DepthClipEnable;
		desc.ScissorEnable;
		desc.MultisampleEnable;
		desc.AntialiasedLineEnable;
	}
	HRESULT hr = Device->CreateRasterizerState(&desc, &_rasterState);
	assert(SUCCEEDED(hr));
}
