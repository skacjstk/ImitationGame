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

	struct COLOR_DATA	// Effect�� ���鶧 ����� ����ü 
	{
		Color color;
		int path;
		float time;			//  dummy���� Time2, 3�� �����
		float dummy1;		// TextureColor.hlsl �� �̷��� �س��µ�
		float dummy2;		// ���߿� Effect ���� �� ����� ��.
	};

	// Shader & Blob & Layout
	ID3D11VertexShader* _vertexShader = nullptr; // Shader�� �ۼ��� File�� interface		//VS
	ID3D11PixelShader* _pixelShader = nullptr; // Shader�� �ۼ��� File�� interface		//PS
	ID3D10Blob* _vsBlob = nullptr;		//�Ʊ� ������� HLSL �� ���̳ʸ� �� ��ȯ	//VS�� ��
	ID3D10Blob* _psBlob = nullptr;		//�Ʊ� ������� HLSL �� ���̳ʸ� �� ��ȯ	//PS�� ��
	ID3D11InputLayout* _inputLayout = nullptr;		//IA	���� --> Line,Rectangle,Triangle

	// CB --> Matrix
	ID3D11Buffer* _matrixBuffer = nullptr;
	ID3D11Buffer* _colorBuffer = nullptr;

	// Texture 
	ID3D11SamplerState* _sampleState = nullptr;	/* Ȯ�볪 ����� ���� Sampling
													hlsl�� samplerState ��Ƴ����� (s0) �ִµ� �װ� interface ���ش�.*/
	ID3D11RasterizerState* _rasterState = nullptr;	// Rasterizer 
	ID3D11BlendState* _blendState = nullptr;		// OM (S + D) ���� �״� �ؾߵǼ� �ϳ� �� ������ �� ���� ����.
	ID3D11BlendState* _blendStateOff = nullptr;
	ID3D11ShaderResourceView* _SRV[2] = { nullptr , };	// Multi�� Texture ȥ���Ͽ� ������ ���� ColorMixing
	// �ؽ��İ� �ϳ��� �ִ°� �ƴ϶� �� ������Ʈ�� ���� �ؽ��ķ� ������ �� �� �� �ִ�.

public:	// C DeC
	TextureShader(wstring shaderFile);
	~TextureShader();
public:	// ���� �ν��Ͻ� �Լ� 
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation);
public:	// Getter
	ID3D11InputLayout* GetLayout() { return _inputLayout; }
public:	// Setter
	void SetShaderResourceView(int id, ID3D11ShaderResourceView* SRV) { _SRV[id] = SRV; }
	void UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3);
private:	// ����� �ν��Ͻ� �Լ�
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateMatrixBuffer();
	void CreateColorBuffer();
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
	bool CheckFile(wstring shaderFileName);

	// �߰� 
	void CreateSampleState();
	void CreateBlendState();
	void CreateRasterState();
};
