#include "ImitationGame/framework.h"
#include <io.h>
#include "ColorShader.h"
/*
������
// 1. VS, PS Function ���� ���� ������ Interface ���ִ� ��
// 2. Draw PipeLine�� Handling ���ִ� ����
// handling = ������ �߻��� ����
*/
ColorShader::ColorShader(wstring shaderFileName)
{
	//��ü ���ϰ�� �ֱ� ������
	wstring temp = L"./Shader/" + shaderFileName;
	CreateShader(temp);
	CreateInputLayout();
	CreateMatrixBuffer();
}

ColorShader::~ColorShader()
{
	SAFE_RELEASE(_vertexShader);		   // c++����->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_pixelShader);			   // Shader Pixel
	SAFE_RELEASE(_vsBlob);				   // HLSL->Binary��ȯ
	SAFE_RELEASE(_psBlob);				   // HLSL->Binary��ȯ
	SAFE_RELEASE(_inputLayout);			   
	SAFE_RELEASE(_matrixBuffer);
}
/*
���� Constant Buffer�� Interface ����.
*/
void ColorShader::Update(Matrix W, Matrix V, Matrix P)
{
	TRANSFORM_DATA temp;
	// �������
	// Sx	?	?	Tx					<---				// Sx	?	?	0
	// ?	Sy	?	Ty										// ?	Sy	?	0
	// ?	?	Sz	Tz		// ? �� rotate �ڸ� 			   	// ?	?	Sz	0	
	// 0	0	0	W	//���� 							   	// Tx	Ty	Tz	W	
	// �̰� original Matrix Format							// DirectX ��ġ���
	// �̷��� ��ȯ�� ����� �Ѵ�
	D3DXMatrixTranspose(&temp.Projection, &P);
	D3DXMatrixTranspose(&temp.View, &V);
	D3DXMatrixTranspose(&temp.World, &W);

	D3D11_MAPPED_SUBRESOURCE subResource;

	// �����ϴ� : Manipulate	, �� ���� ���� ���� �ɸ� D3D11_MAP_WRITE_DISCARD
	DeviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	//������ �ִ� �����͸� write�� �� �ְ� 
	//map�� unmap ���̿��� ������. ���� ���⼭ �����ϸ� �ȴ�.
	{
		memcpy(subResource.pData, &temp, sizeof(TRANSFORM_DATA));
	}
	DeviceContext->Unmap(_matrixBuffer, 0);
}
/*
Shader�� DC���ٰ� Setting.
PipeLine�� �۵�
*/
void ColorShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{	
	// VS�ܰ�
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers �� ���� ��
	// RS: next 

	// PS
	DeviceContext->PSSetShader(_pixelShader, 0, 0);

	// PipeLine �۵� (OM)
	DeviceContext->Draw(VertexCount, StartVertexLocation);
}

void ColorShader::DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation)
{
	// VS�ܰ�
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers �� ���� ��
	// RS: next 

	// PS
	DeviceContext->PSSetShader(_pixelShader, 0, 0);

	// PipeLine �۵� (OM)
	DeviceContext->DrawIndexed(VertexCount, StartVertexLocation, BaseVertexLocation);
}

void ColorShader::CreateShader(wstring shaderFileName)
{
	if (!CheckFile(shaderFileName))
		return;
	HRESULT hr;
	// 1. Shader File�� Compile �ϰ� �� ����� Blob(Binary large object) �� ����
	{
		hr = D3DX11CompileFromFile(
			shaderFileName.c_str(),			// *�ҽ� ����
			0,								// MACRO ��� ���� 
			0,								// Shader���ϵ� include�� �� �� ����. ���� ������
			"VS",							// *HLSL�� ������� function ��
			"vs_5_0",						// ���� ��������, ������ 4_0
			0,								// Flag1
			0,								// Flag2
			0,								// Thread pump ��� ����
			&this->_vsBlob,					// ���̳ʸ� ������ ����� Blob
			&_errorMsgs,								// Error Message (�ް� ������ ID3D10Blob Ÿ�� errorMessage �����)
			0								// HRRESULT 
		);
		if(_errorMsgs)
			OutputErrorMessage(_errorMsgs);
		assert(SUCCEEDED(hr));
	}

	// 1. Shader File�� Compile �ϰ� �� ����� Blob(Binary large object) �� ����
	{
		hr = D3DX11CompileFromFile(
			shaderFileName.c_str(),			// *�ҽ� ����
			0,								// MACRO ��� ���� 
			0,								// Shader���ϵ� include�� �� �� ����. ���� ������
			"PS",							// HLSL�� ������� function ��
			"ps_5_0",						// ���� ��������, ������ 4_0
			0,								// Flag1
			0,								// Flag2
			0,								// Thread pump ��� ����
			&this->_psBlob,					// ���̳ʸ� ������ ����� Blob
			&_errorMsgs,					// Error Message (�ް� ������ ID3D10Blob Ÿ�� errorMessage �����)
			0								// HRRESULT 
		);
		if (_errorMsgs)
			OutputErrorMessage(_errorMsgs);
		assert(SUCCEEDED(hr));
	}
	// ������� Blob�� ���� Shader�� ����
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
	layoutDesc[0].SemanticName = "POSITION";			//Color.hlsl �� 
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

	//Layout ������ ����
	hr = Device->CreateInputLayout(layoutDesc, 2, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_inputLayout);
	assert(SUCCEEDED(hr));	//CreateInputLayout()
}

void ColorShader::CreateMatrixBuffer()
{
	D3D11_BUFFER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;	//GPU �����Ͱ� �Ѿ ��, �����͸� ���� ���� (DYNAMIC - ����)
		desc.ByteWidth = sizeof(TRANSFORM_DATA);	//�ƿ� ����ȭ �ߴ�. ���� ����ü�� ��
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// ������ �����ϱ� ���� ���� ���� ������
	//	D3D11_BIND_VERTEX_BUFFER = 0x1L,	// ��� ���� ���
	//	D3D11_BIND_INDEX_BUFFER = 0x2L,		// 6�� ������ 4���� ���̰�, �ﰢ���� �����ϴ� Index 3D���� ü�� Ŀ
	//	D3D11_BIND_CONSTANT_BUFFER = 0x4L,	// *������ ������� Matrix, float4, 
	//	D3D11_BIND_SHADER_RESOURCE = 0x8L,	// Texture ( bmp, png, �� ���� Resource )
	}

	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &this->_matrixBuffer);
	assert(SUCCEEDED(hr));
}

//���� ����� True ��ȯ
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
