#include "ImitationGame/framework.h"
#include <io.h>
#include "ColorShader.h"
/*
생성자
// 1. VS, PS Function 에서 사용될 변수를 Interface 해주는 것
// 2. Draw PipeLine을 Handling 해주는 역할
// handling = 일종의 추상적 참조
*/
ColorShader::ColorShader(wstring shaderFileName)
{
	//전체 파일경로 넣기 빡세다
	wstring temp = L"./Shader/" + shaderFileName;
	CreateShader(temp);
	CreateInputLayout();
	CreateMatrixBuffer();
}

ColorShader::~ColorShader()
{
	SAFE_RELEASE(_vertexShader);		   // c++정점->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_pixelShader);			   // Shader Pixel
	SAFE_RELEASE(_vsBlob);				   // HLSL->Binary변환
	SAFE_RELEASE(_psBlob);				   // HLSL->Binary변환
	SAFE_RELEASE(_inputLayout);			   
	SAFE_RELEASE(_matrixBuffer);
}
/*
현재 Constant Buffer만 Interface 해줌.
*/
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

	D3D11_MAPPED_SUBRESOURCE subResource;

	// 조작하다 : Manipulate	, 이 순간 쓰기 방지 걸림 D3D11_MAP_WRITE_DISCARD
	DeviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	//기존에 있던 데이터를 write할 수 있게 
	//map과 unmap 사이에서 수행함. 이제 여기서 수정하면 된다.
	{
		memcpy(subResource.pData, &temp, sizeof(TRANSFORM_DATA));
	}
	DeviceContext->Unmap(_matrixBuffer, 0);
}
/*
Shader를 DC에다가 Setting.
PipeLine을 작동
*/
void ColorShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{	
	// VS단계
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers 과 같은 놈
	// RS: next 

	// PS
	DeviceContext->PSSetShader(_pixelShader, 0, 0);

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
	DeviceContext->PSSetShader(_pixelShader, 0, 0);

	// PipeLine 작동 (OM)
	DeviceContext->DrawIndexed(VertexCount, StartVertexLocation, BaseVertexLocation);
}

void ColorShader::CreateShader(wstring shaderFileName)
{
	if (!CheckFile(shaderFileName))
		return;
	HRESULT hr;
	// 1. Shader File을 Compile 하고 그 결과를 Blob(Binary large object) 에 저장
	{
		hr = D3DX11CompileFromFile(
			shaderFileName.c_str(),			// *소스 파일
			0,								// MACRO 사용 안함 
			0,								// Shader파일도 include를 할 수 있음. 역시 사용안함
			"VS",							// *HLSL에 적어놓은 function 명
			"vs_5_0",						// 사용될 프로파일, 에러시 4_0
			0,								// Flag1
			0,								// Flag2
			0,								// Thread pump 사용 안함
			&this->_vsBlob,					// 바이너리 파일이 저장될 Blob
			&_errorMsgs,								// Error Message (받고 싶으면 ID3D10Blob 타입 errorMessage 만들어)
			0								// HRRESULT 
		);
		if(_errorMsgs)
			OutputErrorMessage(_errorMsgs);
		assert(SUCCEEDED(hr));
	}

	// 1. Shader File을 Compile 하고 그 결과를 Blob(Binary large object) 에 저장
	{
		hr = D3DX11CompileFromFile(
			shaderFileName.c_str(),			// *소스 파일
			0,								// MACRO 사용 안함 
			0,								// Shader파일도 include를 할 수 있음. 역시 사용안함
			"PS",							// HLSL에 적어놓은 function 명
			"ps_5_0",						// 사용될 프로파일, 에러시 4_0
			0,								// Flag1
			0,								// Flag2
			0,								// Thread pump 사용 안함
			&this->_psBlob,					// 바이너리 파일이 저장될 Blob
			&_errorMsgs,					// Error Message (받고 싶으면 ID3D10Blob 타입 errorMessage 만들어)
			0								// HRRESULT 
		);
		if (_errorMsgs)
			OutputErrorMessage(_errorMsgs);
		assert(SUCCEEDED(hr));
	}
	// 만들어진 Blob을 통해 Shader를 생성
	{
		hr = Device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), NULL, &_vertexShader);
		assert(SUCCEEDED(hr));
	}
	{
		hr = Device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), NULL, &_pixelShader);
		assert(SUCCEEDED(hr));
	}
}

void ColorShader::CreateInputLayout()
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layoutDesc[2];
	layoutDesc[0].SemanticName = "POSITION";			//Color.hlsl 에 
	layoutDesc[0].SemanticIndex = 0;
	layoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layoutDesc[0].InputSlot = 0;
	layoutDesc[0].AlignedByteOffset = 0;
	layoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[0].InstanceDataStepRate = 0;

	layoutDesc[1].SemanticName = "COLOR";
	layoutDesc[1].SemanticIndex = 0;
	layoutDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layoutDesc[1].InputSlot = 0;
	layoutDesc[1].AlignedByteOffset = 12;
	layoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[1].InstanceDataStepRate = 0;

	//Layout 데이터 만듬
	hr = Device->CreateInputLayout(layoutDesc, 2, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_inputLayout);
	assert(SUCCEEDED(hr));	//CreateInputLayout()
}

void ColorShader::CreateMatrixBuffer()
{
	D3D11_BUFFER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;	//GPU 데이터가 넘어간 후, 데이터를 수정 여부 (DYNAMIC - 가능)
		desc.ByteWidth = sizeof(TRANSFORM_DATA);	//아예 변수화 했다. 보통 구조체로 씀
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// 데이터 수정하기 위한 접근 가능 지정자
	//	D3D11_BIND_VERTEX_BUFFER = 0x1L,	// 모든 정점 사용
	//	D3D11_BIND_INDEX_BUFFER = 0x2L,		// 6개 정점을 4개로 줄이고, 삼각형을 연결하는 Index 3D에서 체감 커
	//	D3D11_BIND_CONSTANT_BUFFER = 0x4L,	// *정점과 관계없는 Matrix, float4, 
	//	D3D11_BIND_SHADER_RESOURCE = 0x8L,	// Texture ( bmp, png, 에 대한 Resource )
	}

	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &this->_matrixBuffer);
	assert(SUCCEEDED(hr));
}

//파일 존재시 True 반환
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
	//	string str;
	//	str.assign(shaderFileName.begin(), shaderFileName.end());
	//	if (_access(str.c_str(), 00) != 0)	// <io.h>
	//	{
	//		flag = false;
	//		str = "Shader File Not Found >>" + str;
	//		MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
	//	}
	return flag;
}

void ColorShader::OutputErrorMessage(ID3D10Blob* errorMsgs)
{
	FILE* op = fopen("./error.txt", "w");
	if (op != NULL)
	{
		fprintf(op, "%s\n", (char*)errorMsgs->GetBufferPointer());
		fclose(op);
	}
	_errorMsgs = nullptr;
}
