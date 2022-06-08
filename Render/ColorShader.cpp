#include "ImitationGame/framework.h"
#include <io.h>
#include "ColorShader.h"

ColorShader::ColorShader(wstring shaderFile)
{
	wstring File = shaderFile;
	if (CheckFile(File))
	{
		CreateShader(File);
		CreateInputLayout();
		CreateMatrixBuffer();
		CreateColorBuffer();
	}
}

ColorShader::~ColorShader()
{
	SAFE_RELEASE(_vertexShader);			// c++정점->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_pixelShader);				// Shader Pixel
	SAFE_RELEASE(_vsBlob);					// HLSL->Binary변환
	SAFE_RELEASE(_psBlob);					// HLSL->Binary변환
	SAFE_RELEASE(_inputLayout);				// IA
	SAFE_RELEASE(_matrixBuffer);
	SAFE_RELEASE(_colorBuffer);
}

/// <summary>
/// Matrix를 Shader에 넘겨주는 역할 
/// </summary>
/// <param name="W">World 좌표</param>
/// <param name="V">View 좌표</param>
/// <param name="P">Projection 좌표</param>
void ColorShader::Update(Matrix W, Matrix V, Matrix P)
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
/// <summary>
/// PipeLine 에서 받아온 IA(Line)을 -> VS -> PS 단계를 거친 후 OM 실행 
/// </summary>
/// <param name="VertexCount">Count of Vertex</param>
/// <param name="StartVertexLocation">Start index of VertexCount</param>
void ColorShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// VS단계
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers 과 같은 놈
	// RS: next 

	// PS
	DeviceContext->PSSetShader(_pixelShader, nullptr, 0);	// 원래 0이었는데 nullptr로 바뀐건 무슨 의미일까

	// PipeLine 작동 (OM)
	DeviceContext->Draw(VertexCount, StartVertexLocation);
}

void ColorShader::DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation)
{	
	// VS단계
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers 과 같은 놈
	// RS: next 

	// PS
	DeviceContext->PSSetShader(_pixelShader, nullptr, 0);	// 원래 0이었는데 nullptr로 바뀐건 무슨 의미일까

	// PipeLine 작동 (OM)
	DeviceContext->DrawIndexed(VertexCount, StartVertexLocation, BaseVertexLocation);
}

/// <summary>
/// Create Shader and Blob
/// </summary>
/// <param name="shaderFile">FilePath + FileName</param>
void ColorShader::CreateShader(wstring shaderFile)
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
/// Create InputLayout
/// </summary>
void ColorShader::CreateInputLayout()
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

	layout[1].SemanticName = "COLOR";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Vector3 float x, y, z
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = 12;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	hr = Device->CreateInputLayout(layout, 2, _vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		&this->_inputLayout);
	assert(SUCCEEDED(hr));
}


void ColorShader::CreateMatrixBuffer()
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
/// <summary>
/// Color를 변경하고자 할 때 ColorBuffer만 따로 
/// </summary>
void ColorShader::CreateColorBuffer()
{
	// 위에 선언된 데이터를 VertextBuffer에 Setting
	// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
	// 무조건 Desctrpion이 붙는다
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(TRANSFORM_DATA);              // Vector3,Color ( 3*4 + 4*4)
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

void ColorShader::OutputErrorMessage(ID3D10Blob* errorMsgs)
{
	FILE* op = fopen("./Error/error.txt", "w");
	if (op != NULL)
	{
		wstring wstr = (WCHAR*)errorMsgs->GetBufferPointer();
		fprintf(op, "%s\n", (char*)errorMsgs->GetBufferPointer());
		MessageBoxW(NULL, wstr.c_str(), L"Output Error Message", MB_OK);
		fclose(op);
	}
	system("NotePad.exe ./error.txt");
	errorMsgs = nullptr;
}

bool ColorShader::CheckFile(wstring shaderFileName)
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
