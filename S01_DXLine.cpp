#include "./ImitationGame/framework.h"
#include "S01_DXLine.h"

S01_DXLine::S01_DXLine()
{
	CreateVertexBuffer();	//������ ����� �����͸� _vertexBuffer�� I/F
	CreateShader();			//effect.hlsl -> blob -> shader
	CreateInputLayout();	//������ ���� Layout�� ����
}

S01_DXLine::~S01_DXLine()
{
	SAFE_RELEASE(_vertexBuffer);	    // c++����->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_vertexShader);	    // Shader�� �ۼ��� ������ I/F
	SAFE_RELEASE(_pixelShader);		    // Shader Pixel
	SAFE_RELEASE(_vsBlob);			    // HLSL->Binary��ȯ
	SAFE_RELEASE(_psBlob);			    // HLSL->Binary��ȯ
	SAFE_RELEASE(_inputLayout);
}
/*
��ǥ, Matrix(Scale, Rotate)
�� �����༭ �װɷ� ������Ʈ
*/
void S01_DXLine::Update()
{
}
/*
������� ������� (IA) ������ �̿��ؼ� DX�� PipeLine�� ����
Rendering�� �Ѵ�.
*/
void S01_DXLine::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// IA �ܰ�: Device���� �غ��� (_vertexBuffer)�� -->DeviceContext�� Setting

	DeviceContext->IASetVertexBuffers(0, // startSlot
		1,	// buffer�� ����
		&_vertexBuffer,		// ���� ������
		&stride,			// ���� �������� Size
		&offset				// ������		
		);
	DeviceContext->IASetInputLayout(_inputLayout);
	//D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	// �갡 �� �׷��� ���� �𸣴µ�, �� �׸� �� �� ���� ���� ����� ��
	// VS
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->PSSetShader(_pixelShader, 0, 0);


	// Output Meger �ܰ�
	//Rendering
	DeviceContext->Draw(4, 0);
}

void S01_DXLine::ChangeScene()
{
}
/*
IA -> Input Assembly �ܰ� (�Է� ���� �ܰ�)

			|						--> �ð����
		1	|	2
--------------------------------	x	  y		 z
		0	|	3				0: -0.5f, -0.5f, 0.0f
			|					1: -0.5f, +0.5f, 0.0f
								2: +0.5f, +0.5f, 0.0f
								3: +0.5f, -0.5f, 0.0f

*/
void S01_DXLine::CreateVertexBuffer()
{
	// 1. ������ ����� (�ð����{���� CW}  DirectX)
	// �ݴ�� CCW (counter clock watch) (OpenGL))
	_vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f); //0
	_vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f); //1
	_vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f); //2
	_vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f); //3

	_vertices[0].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	_vertices[1].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	_vertices[2].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	_vertices[3].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

	_vertexCount = sizeof(_vertices) / sizeof(Vertex);	//������ ������ �ȴ�.
	//2. CPU�� �ڿ��� GPU �� IA�ܰ迡 �Ѱ��ֱ� ���� ��ȯ �۾��� �����Ѵ�
	D3D11_BUFFER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;	//GPU �����Ͱ� �Ѿ ��, �����͸� ���� ���� (default = �Ұ�)
		desc.ByteWidth = sizeof(Vertex) * _vertexCount;	//_vertices�� 4���� ������ ���� ����ü�� �� 
											// Class�� �Ǳ�� ������, ByteWidth ����� ��ƴ�. (������ ��������?)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �� desc�� Vertex Buffer
	//	D3D11_BIND_VERTEX_BUFFER = 0x1L,	// ���� ���
	//	D3D11_BIND_INDEX_BUFFER = 0x2L,		// 6�� ������ 4���� ���̰�, �ﰢ���� �����ϴ� Index 3D���� ü�� Ŀ
	//	D3D11_BIND_CONSTANT_BUFFER = 0x4L,	// ������ ������� Matrix, float4, 
	//	D3D11_BIND_SHADER_RESOURCE = 0x8L,	// Texture ( bmp, png, �� ���� Resource )
	}
	D3D11_SUBRESOURCE_DATA data;
	{
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = _vertices;	//ù��° ������ ������, 
	}
	// �ڿ��� Device�κ��� ����� ( SwapChain, Device, DeviceContext, RTV) 
	// Device�� CPU�� ������ ,DC�� GPU�� ������.
	// �ᱹ CreateBuffer�Ҷ� �ڿ��� Device�κ��� �����.
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->_vertexBuffer);
	// �� Com �������̽� buffer�� data, desc ������ ���� ���� ������ ������.
	assert(SUCCEEDED(hr));
}

/*
// IA -> VS -> RS -> PS - OM
VS �ܰ�
1. D3X11CompileFromFile() : HLSL File�� ������
*/
void S01_DXLine::CreateShader()
{
	HRESULT hr;
	// 1. Shader File�� Compile �ϰ� �� ����� Blob(Binary large object) �� ����
	{
		hr = D3DX11CompileFromFile(
			L"../Shader/Effect.hlsl",		//�ҽ� ����
			0,								// MACRO ��� ���� 
			0,								// Shader���ϵ� include�� �� �� ����. ���� ������
			"VS",							// HLSL�� ������� function ��
			"vs_5_0",						// ���� ��������, ������ 4_0
			0,								// Flag1
			0,								// Flag2
			0,								// Thread pump ��� ����
			&this->_vsBlob,					// ���̳ʸ� ������ ����� Blob
			&_errorMsgs,					// Error Message (�ް� ������ ID3D10Blob Ÿ�� errorMessage �����)
			0								// HRRESULT 
		);
		if (_errorMsgs)
			OutputErrorMessage(_errorMsgs);
		assert(SUCCEEDED(hr));
	}

	// 1. Shader File�� Compile �ϰ� �� ����� Blob(Binary large object) �� ����
	{
		hr = D3DX11CompileFromFile(
			L"../Shader/Effect.hlsl",		//�ҽ� ����
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
		if(_errorMsgs)
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
/*
 IA -> Vertex Stage �� ������ �ѱ� ��
 Shader�� ���� ������ ����Ѵ�.
*/
void S01_DXLine::CreateInputLayout()
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{
			"POSITION",	// Semantic Name
			0,		// Semantic Index
			DXGI_FORMAT_R32G32B32_FLOAT,		// DXGI_FORMAT
			0,		// InputSlot
			0,		// AllignedByteOffset	vertex�� ó���� �޴� ��ġ�� offset
			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass 
			0		// InstanceDataStepRate
		},	//position�� ������ ����
		{
			"COLOR",	// Semantic Name
			0,		// Semantic Index
			DXGI_FORMAT_R32G32B32_FLOAT,		// DXGI_FORMAT
			0,		// InputSlot
			12,		// AllignedByteOffset	(Vector3, Color) Vector3�� 12����Ʈ ��
			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass 
			0		// InstanceDataStepRate
		}	//Color�� ������ ����
	};
	//Layout ������ ����
	hr = Device->CreateInputLayout(layoutDesc, 2, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_inputLayout);
	assert(SUCCEEDED(hr));	//CreateInputLayout()
}
/*
 HLSL ������ �� error �� �߻��ϸ� 
 �� ������ File�� ������
*/
void S01_DXLine::OutputErrorMessage(ID3D10Blob* errorMsgs)
{
	FILE* op;
	fopen_s(&op, "./error.txt", "w");	// ���� ��带 op�� �ϰ� �б⸦ fp�� �߾��ٰ� ��. 
	if (op != NULL)
	{
		fprintf(op, "%s\n", (char*)errorMsgs->GetBufferPointer());
		fclose(op);
	}
	_errorMsgs = nullptr;
}


/*
//���: ������ class �ּҰ� �ȳ����°�, ����ȭ�� �Ǿ�������. 
������ class �� ���� �׿� �ּҰ� ������
class Test2
{
public:
	int x, y;
public:
	Test2() {};
	~Test2() {};
};
class Test
{
public:
	int x, y;
	Test2 C;
public:
	Test() {};
	~Test() {};
};



	Test AA;

	FILE *op;
	fopen_s(&op,"./test.bi", "wb");

	fwrite(&AA, sizeof(Test), 1, op);
	fclose(op);

	printf("%d\n", sizeof(Test));
*/
