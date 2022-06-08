#pragma once
class TextureShader
{
private:
	struct TRANSFORM_DATA	// CB_Buffer Matrix
	{
		Matrix World;
		Matrix View;
		Matrix Projection;
	};

	struct COLOR_DATA	// Effect를 만들때 사용할 구조체 
	{
		Color color;
		int path;
		float time;			//  dummy들은 Time2, 3를 담당함
		float dummy1;		// TextureColor.hlsl 을 이렇게 해놨는데
		float dummy2;		// 나중에 Effect 만들 때 사용할 것.
	};

	// Shader & Blob & Layout
	ID3D11VertexShader* _vertexShader = nullptr; // Shader로 작성된 File을 interface		//VS
	ID3D11PixelShader* _pixelShader = nullptr; // Shader로 작성된 File을 interface		//PS
	ID3D10Blob* _vsBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환	//VS가 들어간
	ID3D10Blob* _psBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환	//PS가 들어간
	ID3D11InputLayout* _inputLayout = nullptr;		//IA	정점 --> Line,Rectangle,Triangle

	// CB --> Matrix
	ID3D11Buffer* _matrixBuffer = nullptr;
	ID3D11Buffer* _colorBuffer = nullptr;

	// Texture 
	ID3D11SamplerState* _sampleState = nullptr;	/* 확대나 축소할 때의 Sampling
													hlsl에 samplerState 잡아놓은거 (s0) 있는데 그거 interface 해준다.*/
	ID3D11RasterizerState* _rasterState = nullptr;	// Rasterizer 
	ID3D11BlendState* _blendState = nullptr;		// OM (S + D) 껏다 켰다 해야되서 하나 더 만들어야 할 수도 있음.
	ID3D11BlendState* _blendStateOff = nullptr;
	ID3D11ShaderResourceView* _SRV[2] = { nullptr , };	// Multi로 Texture 혼합하여 색깔을 만듬 ColorMixing
	// 텍스쳐가 하나만 있는게 아니라 한 오브젝트를 여러 텍스쳐로 만들어야 할 수 도 있다.

public:	// C DeC
	TextureShader(wstring shaderFile);
	~TextureShader();
public:	// 공개 인스턴스 함수 
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation);
public:	// Getter
	ID3D11InputLayout* GetLayout() { return _inputLayout; }
public:	// Setter
	void SetShaderResourceView(int id, ID3D11ShaderResourceView* SRV) { _SRV[id] = SRV; }
	void UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3);
private:	// 비공개 인스턴스 함수
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateMatrixBuffer();
	void CreateColorBuffer();
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
	bool CheckFile(wstring shaderFileName);

	// 추가 
	void CreateSampleState();
	void CreateBlendState();
	void CreateRasterState();
};
