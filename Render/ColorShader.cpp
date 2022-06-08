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
	SAFE_RELEASE(_vertexShader);			// c++����->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_pixelShader);				// Shader Pixel
	SAFE_RELEASE(_vsBlob);					// HLSL->Binary��ȯ
	SAFE_RELEASE(_psBlob);					// HLSL->Binary��ȯ
	SAFE_RELEASE(_inputLayout);				// IA
	SAFE_RELEASE(_matrixBuffer);
	SAFE_RELEASE(_colorBuffer);
}

/// <summary>
/// Matrix�� Shader�� �Ѱ��ִ� ���� 
/// </summary>
/// <param name="W">World ��ǥ</param>
/// <param name="V">View ��ǥ</param>
/// <param name="P">Projection ��ǥ</param>
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

	// ��� ���ۿ� ���ִ� ������ �ϸ� �ȴ�.
	// MAP -> UNMAP ���̿��� ����
	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, &temp, sizeof(TRANSFORM_DATA));
	}
	DeviceContext->Unmap(_matrixBuffer, 0);
}
/// <summary>
/// PipeLine ���� �޾ƿ� IA(Line)�� -> VS -> PS �ܰ踦 ��ģ �� OM ���� 
/// </summary>
/// <param name="VertexCount">Count of Vertex</param>
/// <param name="StartVertexLocation">Start index of VertexCount</param>
void ColorShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// VS�ܰ�
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers �� ���� ��
	// RS: next 

	// PS
	DeviceContext->PSSetShader(_pixelShader, nullptr, 0);	// ���� 0�̾��µ� nullptr�� �ٲ�� ���� �ǹ��ϱ�

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
	DeviceContext->PSSetShader(_pixelShader, nullptr, 0);	// ���� 0�̾��µ� nullptr�� �ٲ�� ���� �ǹ��ϱ�

	// PipeLine �۵� (OM)
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

	// HLSL( VS )�� �������� Blob�� ����
	// Binary Large Object
	// Shader�� �����ϱ� ���� ���� Blob�����ؾ� �Ѵ�
	{
		hr = D3DX11CompileFromFile(
			shaderFile.c_str(),   // Source File
			NULL,                      // MACRO
			NULL,                      // include
			"VS",                      // Effect.hlsk�� ���ǵ� function��
			"vs_5_0",                  // hlsl�� ���Ǵ� version��
			0,                         // Flag1
			0,                         // Flag2
			NULL,                      // ThredPump ������
			&this->_vsBlob,          // �������� --> Blob�� ����
			&pErrorMsgs,
			&result
		);

		if (pErrorMsgs)
			OutputErrorMessage(pErrorMsgs);


		assert(SUCCEEDED(hr));
	}

	pErrorMsgs = NULL;

	// HLSL( PS )�� �������� Blob�� ����
	// Binary Large Object
	// Shader�� �����ϱ� ���� ���� Blob�����ؾ� �Ѵ�
	{
		hr = D3DX11CompileFromFile(
			shaderFile.c_str(),   // Source File
			NULL,                      // MACRO
			NULL,                      // include
			"PS",                      // Effect.hlsk�� ���ǵ� function��
			"ps_5_0",                  // hlsl�� ���Ǵ� version��
			0,                         // Flag1
			0,                         // Flag2
			NULL,                      // ThredPump ������
			&this->_psBlob,          // �������� --> Blob�� ����
			&pErrorMsgs,
			&result
		);

		if (pErrorMsgs)
			OutputErrorMessage(pErrorMsgs);


		assert(SUCCEEDED(hr));
	}

	// ������� Blob�� ���� Shader����
	// VertexShader����
	{
		hr = Device->CreateVertexShader(_vsBlob->GetBufferPointer(),
			_vsBlob->GetBufferSize(),
			NULL,
			&this->_vertexShader);
		assert(SUCCEEDED(hr));
	}

	// ������� Blob�� ���� Shader����
	// PixelShader����
	{
		hr = Device->CreatePixelShader(_psBlob->GetBufferPointer(),
			_psBlob->GetBufferSize(),
			NULL,
			&this->_pixelShader);
		assert(SUCCEEDED(hr));
	}

	// Shader ���α׷��� ����,������� ����������
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
	// ���� ����� �����͸� VertextBuffer�� Setting
	// DX���� Craete,Get()�� ����Ҷ� ������Ī �տ� "I"�ڰ� ������
	// ������ Desctrpion�� �ٴ´�
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // ������ 0�� �ʱ�ȭ
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *�������Ŀ� �����Ҽ�  �Ѵ�
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *�������Ŀ� �����Ҽ�  �Ѵ�
		desc.ByteWidth = sizeof(TRANSFORM_DATA);       // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     ����
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      �ߺ��Ǵ� ������ Index�� �����Ͽ� ���
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   ���Buffer, User�� HLSL�� ������ ������
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile

	}
	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &_matrixBuffer);
	//	printf("%d\n", sizeof(Vertex));
	assert(SUCCEEDED(hr));
}
/// <summary>
/// Color�� �����ϰ��� �� �� ColorBuffer�� ���� 
/// </summary>
void ColorShader::CreateColorBuffer()
{
	// ���� ����� �����͸� VertextBuffer�� Setting
	// DX���� Craete,Get()�� ����Ҷ� ������Ī �տ� "I"�ڰ� ������
	// ������ Desctrpion�� �ٴ´�
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // ������ 0�� �ʱ�ȭ
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *�������Ŀ� �����Ҽ�  �Ѵ�
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *�������Ŀ� �����Ҽ�  �Ѵ�
		desc.ByteWidth = sizeof(TRANSFORM_DATA);              // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     ����
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      �ߺ��Ǵ� ������ Index�� �����Ͽ� ���
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   ���Buffer, User�� HLSL�� ������ ������
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
