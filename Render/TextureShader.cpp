#include "ImitationGame/framework.h"
#include <io.h>
#include "TextureShader.h"

TextureShader::TextureShader(wstring shaderFile)
{
	//	wstring File = SHADER_FOLDER + shaderFile;
	wstring File = shaderFile;

	// �ʱ�ȭ
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
	SAFE_RELEASE(_vertexShader);			// c++����->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_pixelShader);				// Shader Pixel
	SAFE_RELEASE(_vsBlob);					// HLSL->Binary��ȯ
	SAFE_RELEASE(_psBlob);					// HLSL->Binary��ȯ
	SAFE_RELEASE(_inputLayout);				// IA
	SAFE_RELEASE(_matrixBuffer);
	SAFE_RELEASE(_colorBuffer);
	SAFE_RELEASE(_sampleState);
	SAFE_RELEASE(_rasterState);
	SAFE_RELEASE(_blendState);
	SAFE_RELEASE(_blendStateOff);
	//SRV�� SRV�� ���� SRVManager �ʿ��� ���� �Ŷ� ��� �ȴ�.
}
// ColorShader �� �Ȱ���.
void TextureShader::Update(Matrix W, Matrix V, Matrix P)
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
// ColorShader ��� (RS �߰�)
void TextureShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// VS�ܰ�
	DeviceContext->VSSetShader(_vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);	// IASetVertexBuffers �� ���� ��

	// RS: �ܰ� 
	DeviceContext->RSSetState(_rasterState);
	// PS
	DeviceContext->PSSetShader(_pixelShader, nullptr, 0);	// ���� 0�̾��µ� nullptr�� �ٲ�� ���� �ǹ��ϱ�
	DeviceContext->PSSetSamplers(0, 1, &_sampleState);	// Line ������ ? �߰� 

	if (_SRV[1])
		DeviceContext->PSSetShaderResources(0, 2, _SRV); // 2���� ����
	else
		DeviceContext->PSSetShaderResources(0, 1, _SRV); // 1���� ����	( 0429 Get �Ǽ� �߾��� ) 

	DeviceContext->PSSetConstantBuffers(1, 1, &_colorBuffer);	// VS ���� 0���� �����ϱ�, 1������ �ְڴ� �̷��� 

	// PipeLine �۵� (OM)
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

	// Map �� Unmap ���̿��� GPU�� Vertex ������ Update
	// ��� ���ۿ� ���ִ� ������ �ϸ� �ȴ�.
	// MAP -> UNMAP ���̿��� ����
	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, &temp, sizeof(COLOR_DATA));
	}
	DeviceContext->Unmap(_colorBuffer, 0);
}

///
/// VS, PS�� ���� ���̴� ����
/// Blob(���Ϸ� ������) --> Shader ���� ������� �ٸ��� ����. ColorShader ���� ������
///
void TextureShader::CreateShader(wstring shaderFile)
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
/// ������ Vertex �� ������ ��� �Ǿ��ֳĸ� �˷���.
/// �������� shader�� ����� ����ؾ� �� ���� ����.
/// Uv�� Color�� �ٸ���, �׷��� InputLayout ������ -> ��� Color�ε� ��� TEXCOORD0(uv) �׶���.
/// Uv �� TEXCOORD1 �ϳ� �� ����� inputLayout�� 3���� ����? �׷� ����.
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
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;  // �ؽ��Ŀ��� �� �Ǽ� ��ġ��, TEXCOORD �� Vector2
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = 12;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;
	// 2�� �̻��� ��� ���� 
	hr = Device->CreateInputLayout(layout, 2, _vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		&this->_inputLayout);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateMatrixBuffer()
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

void TextureShader::CreateColorBuffer()
{	
	// ���� ����� �����͸� VertextBuffer�� Setting
// DX���� Craete,Get()�� ����Ҷ� ������Ī �տ� "I"�ڰ� ������
// ������ Desctrpion�� �ٴ´�
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // ������ 0�� �ʱ�ȭ
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *�������Ŀ� �����Ҽ�  �Ѵ�
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *�������Ŀ� �����Ҽ�  �Ѵ�
		desc.ByteWidth = sizeof(COLOR_DATA);       // Vector3,Color ( 3*4 + 4*4)
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
		// ����ȭ�� ��ǥ (NDC: normalized device coordinate) �� �Ѿ��� �� ��� �� �� 
		// WRAP		: �����ȼ��� ������ �κ��� ä���
		// CLAMP	: �Ǹ����� �ȼ��� ä���
		// MIRROR	: ������ �ȼ��� ������Ų��. 
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;					//	D3D11_TEXTURE_ADDRESS_MODE
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;					//  D3D11_TEXTURE_ADDRESS_MODE
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;					//  D3D11_TEXTURE_ADDRESS_MODE

		desc.BorderColor[0] = 1; // �ܰ����� ���� white ��� �� (�����)
		desc.BorderColor[1] = 0; // �ܰ����� ���� white ��� �� (�����)
		desc.BorderColor[2] = 0; // �ܰ����� ���� white ��� �� (�����)
		desc.BorderColor[3] = 1; // �ܰ����� ���� white ��� �� (�����)

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// ���� / ���� �����͸� ���ϴ� Flag,
		// ���� SAMPLE�� 4��(BorderColor�� ��) ���� ���ϴµ�, �׻� ���϶� 

		// MIN --> �̹��� ���
		// MAG --> �̹��� Ȯ�� 
		// �̹����� Ȯ���ϰų� ����� �� �̸� 1/4 ũ��� �̹����� �����Ѵ�. 
		// POINT : 2���� pixel�� �ִٸ� �ϳ��� ���.
		// LINEAR : 2���� pixel�� ��հ� 
		desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		desc.MaxAnisotropy = 16;	// ���漺 ���� 
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		//	FLOAT MipLODBias;
	}
	HRESULT hr = Device->CreateSamplerState(&desc, &_sampleState);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateBlendState()
{
	// ���ĺ��� �Ҳ��� ������ �̷��� ����
	D3D11_BLEND_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;	// �ڿ� �׵θ��� �����ϰڴ�.
		desc.IndependentBlendEnable = true;	// �������� blend�� �ϰڴ�. false �� ��� ���İ��� ȥ�յ��� ����.
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;	// �ǹ�: ��¶�� �갡 �����̶�°ǰ�?	�갡 0.9 
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//  ��� 0.9 --> 0.1 �� �ٲ� 
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;	// operation add 

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	// ������ �ڰ� ��¦ ���̰� ������� 
	HRESULT hr = Device->CreateBlendState(&desc, &_blendState);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateRasterState()
{
	// ȯ�濡 ���� ���߿� desc�� �ٲ� �� �־�, 
	D3D11_RASTERIZER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;	// ���θ� ĥ�� ���� : ĥ��
		desc.CullMode = D3D11_CULL_NONE;	// 3D ���� �Ⱥ��̴� �κ��� ����µ�, �� �κ��� ä���� ������ ���� ��
		// culling: �߽�������. 3D���� �Ⱥ��̴� ������ �߽��� �׸� �׸��� ���� ����� ǥ��
		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_cull_mode
		// None�� �׻� �׸��� ��
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
