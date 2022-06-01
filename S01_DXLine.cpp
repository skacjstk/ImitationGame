#include "./ImitationGame/framework.h"
#include "S01_DXLine.h"

S01_DXLine::S01_DXLine()
{
	CreateVertexBuffer();	//정점을 만들고 데이터를 _vertexBuffer에 I/F
	CreateShader();			//effect.hlsl -> blob -> shader
	CreateInputLayout();	//정점에 대한 Layout을 만듬
}

S01_DXLine::~S01_DXLine()
{
	SAFE_RELEASE(_vertexBuffer);	    // c++정점->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_RELEASE(_vertexShader);	    // Shader로 작성된 파일을 I/F
	SAFE_RELEASE(_pixelShader);		    // Shader Pixel
	SAFE_RELEASE(_vsBlob);			    // HLSL->Binary변환
	SAFE_RELEASE(_psBlob);			    // HLSL->Binary변환
	SAFE_RELEASE(_inputLayout);
}
/*
좌표, Matrix(Scale, Rotate)
를 보내줘서 그걸로 업데이트
*/
void S01_DXLine::Update()
{
}
/*
현재까지 만들어진 (IA) 정보를 이용해서 DX의 PipeLine에 보내
Rendering을 한다.
*/
void S01_DXLine::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// IA 단계: Device에서 준비한 (_vertexBuffer)를 -->DeviceContext에 Setting

	DeviceContext->IASetVertexBuffers(0, // startSlot
		1,	// buffer의 갯수
		&_vertexBuffer,		// 보낼 데이터
		&stride,			// 보낼 데이터의 Size
		&offset				// 오프셋		
		);
	DeviceContext->IASetInputLayout(_inputLayout);
	//D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	// 얘가 뭘 그려야 할지 모르는데, 뭘 그릴 지 에 대한 것을 기술해 줌
	// VS
	DeviceContext->VSSetShader(_vertexShader, 0, 0);
	DeviceContext->PSSetShader(_pixelShader, 0, 0);


	// Output Meger 단계
	//Rendering
	DeviceContext->Draw(4, 0);
}

void S01_DXLine::ChangeScene()
{
}
/*
IA -> Input Assembly 단계 (입력 조립 단계)

			|						--> 시계방향
		1	|	2
--------------------------------	x	  y		 z
		0	|	3				0: -0.5f, -0.5f, 0.0f
			|					1: -0.5f, +0.5f, 0.0f
								2: +0.5f, +0.5f, 0.0f
								3: +0.5f, -0.5f, 0.0f

*/
void S01_DXLine::CreateVertexBuffer()
{
	// 1. 정점을 만든다 (시계방향{이하 CW}  DirectX)
	// 반대는 CCW (counter clock watch) (OpenGL))
	_vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f); //0
	_vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f); //1
	_vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f); //2
	_vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f); //3

	_vertices[0].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	_vertices[1].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	_vertices[2].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	_vertices[3].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

	_vertexCount = sizeof(_vertices) / sizeof(Vertex);	//작지만 않으면 된다.
	//2. CPU의 자원을 GPU 중 IA단계에 넘겨주기 위한 변환 작업을 수행한다
	D3D11_BUFFER_DESC desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;	//GPU 데이터가 넘어간 후, 데이터를 수정 여부 (default = 불가)
		desc.ByteWidth = sizeof(Vertex) * _vertexCount;	//_vertices를 4개만 썻었지 보통 구조체로 씀 
											// Class도 되기는 되지만, ByteWidth 계산이 어렵다. (이제는 쉬울지도?)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 이 desc는 Vertex Buffer
	//	D3D11_BIND_VERTEX_BUFFER = 0x1L,	// 정점 사용
	//	D3D11_BIND_INDEX_BUFFER = 0x2L,		// 6개 정점을 4개로 줄이고, 삼각형을 연결하는 Index 3D에서 체감 커
	//	D3D11_BIND_CONSTANT_BUFFER = 0x4L,	// 정점과 관계없는 Matrix, float4, 
	//	D3D11_BIND_SHADER_RESOURCE = 0x8L,	// Texture ( bmp, png, 에 대한 Resource )
	}
	D3D11_SUBRESOURCE_DATA data;
	{
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = _vertices;	//첫번째 번지수 보내기, 
	}
	// 자원은 Device로부터 만든다 ( SwapChain, Device, DeviceContext, RTV) 
	// Device는 CPU에 가깝고 ,DC는 GPU에 가깝다.
	// 결국 CreateBuffer할때 자원은 Device로부터 만든다.
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->_vertexBuffer);
	// 이 Com 인터페이스 buffer에 data, desc 절차에 의해 여러 정보를 복사함.
	assert(SUCCEEDED(hr));
}

/*
// IA -> VS -> RS -> PS - OM
VS 단계
1. D3X11CompileFromFile() : HLSL File을 컴파일
*/
void S01_DXLine::CreateShader()
{
	HRESULT hr;
	// 1. Shader File을 Compile 하고 그 결과를 Blob(Binary large object) 에 저장
	{
		hr = D3DX11CompileFromFile(
			L"../Shader/Effect.hlsl",		//소스 파일
			0,								// MACRO 사용 안함 
			0,								// Shader파일도 include를 할 수 있음. 역시 사용안함
			"VS",							// HLSL에 적어놓은 function 명
			"vs_5_0",						// 사용될 프로파일, 에러시 4_0
			0,								// Flag1
			0,								// Flag2
			0,								// Thread pump 사용 안함
			&this->_vsBlob,					// 바이너리 파일이 저장될 Blob
			&_errorMsgs,					// Error Message (받고 싶으면 ID3D10Blob 타입 errorMessage 만들어)
			0								// HRRESULT 
		);
		if (_errorMsgs)
			OutputErrorMessage(_errorMsgs);
		assert(SUCCEEDED(hr));
	}

	// 1. Shader File을 Compile 하고 그 결과를 Blob(Binary large object) 에 저장
	{
		hr = D3DX11CompileFromFile(
			L"../Shader/Effect.hlsl",		//소스 파일
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
		if(_errorMsgs)
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
/*
 IA -> Vertex Stage 로 정보를 넘길 때
 Shader가 받을 정보를 기술한다.
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
			0,		// AllignedByteOffset	vertex가 처음에 받는 위치의 offset
			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass 
			0		// InstanceDataStepRate
		},	//position의 데이터 구성
		{
			"COLOR",	// Semantic Name
			0,		// Semantic Index
			DXGI_FORMAT_R32G32B32_FLOAT,		// DXGI_FORMAT
			0,		// InputSlot
			12,		// AllignedByteOffset	(Vector3, Color) Vector3가 12바이트 라서
			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass 
			0		// InstanceDataStepRate
		}	//Color의 데이터 구성
	};
	//Layout 데이터 만듬
	hr = Device->CreateInputLayout(layoutDesc, 2, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_inputLayout);
	assert(SUCCEEDED(hr));	//CreateInputLayout()
}
/*
 HLSL 컴파일 중 error 가 발생하면 
 그 내용을 File에 저장함
*/
void S01_DXLine::OutputErrorMessage(ID3D10Blob* errorMsgs)
{
	FILE* op;
	fopen_s(&op, "./error.txt", "w");	// 쓰기 모드를 op로 하고 읽기를 fp로 했었다고 함. 
	if (op != NULL)
	{
		fprintf(op, "%s\n", (char*)errorMsgs->GetBufferPointer());
		fclose(op);
	}
	_errorMsgs = nullptr;
}


/*
//비고: 이제는 class 주소가 안나오는게, 최적화가 되었나보다. 
옛날엔 class 로 인한 잉여 주소값 나왔음
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
