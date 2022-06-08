#pragma once
/// <summary>
/// ///////
/// Line, Rectangle, Triangle�� ���� Shader
/// </summary>
class ColorShader
{
private:
	struct TRANSFORM_DATA	// CB_Buffer Matrix
	{
		Matrix World;
		Matrix View;
		Matrix Projection;
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

public:	// C DeC
	ColorShader(wstring shaderFile);
	~ColorShader();
public:	// ���� �ν��Ͻ� �Լ� 
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation);
public:	// Getter
	ID3D11InputLayout* GetLayout() { return _inputLayout; }
private:	// ����� �ν��Ͻ� �Լ�
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateMatrixBuffer();
	void CreateColorBuffer();
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
	bool CheckFile(wstring shaderFileName);
};
