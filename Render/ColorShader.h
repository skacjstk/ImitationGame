#pragma once
/// <summary>
/// ///////
/// Line, Rectangle, Triangle에 대한 Shader
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
	ID3D11VertexShader* _vertexShader = nullptr; // Shader로 작성된 File을 interface		//VS
	ID3D11PixelShader* _pixelShader = nullptr; // Shader로 작성된 File을 interface		//PS
	ID3D10Blob* _vsBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환	//VS가 들어간
	ID3D10Blob* _psBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환	//PS가 들어간
	ID3D11InputLayout* _inputLayout = nullptr;		//IA	정점 --> Line,Rectangle,Triangle

	// CB --> Matrix
	ID3D11Buffer* _matrixBuffer = nullptr;
	ID3D11Buffer* _colorBuffer = nullptr;

public:	// C DeC
	ColorShader(wstring shaderFile);
	~ColorShader();
public:	// 공개 인스턴스 함수 
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation);
public:	// Getter
	ID3D11InputLayout* GetLayout() { return _inputLayout; }
private:	// 비공개 인스턴스 함수
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateMatrixBuffer();
	void CreateColorBuffer();
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
	bool CheckFile(wstring shaderFileName);
};
