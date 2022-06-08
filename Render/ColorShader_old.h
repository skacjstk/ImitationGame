#pragma once
class ColorShader
{
private:	// private instance variable
	struct TRANSFORM_DATA
	{
		Matrix World;	// 자기 자신 Scale, Position, Rotate도 될 수 있다. 그런 행렬 데이터 
		Matrix View;	// Camera Position
		Matrix Projection;	// 직교투영 (사영)
	};
	//Buffer는 Scene마다 달라 Scene 시점에서 구현 
	ID3D11VertexShader* _vertexShader = nullptr; // Shader로 작성된 File을 interface		//VS
	ID3D11PixelShader* _pixelShader = nullptr; // Shader로 작성된 File을 interface		//PS
	ID3D10Blob* _vsBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환	//VS가 들어간
	ID3D10Blob* _psBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환	//PS가 들어간
	ID3D11InputLayout* _inputLayout = nullptr;		//IA	정점 --> Line,Rectangle,Triangle
	ID3D10Blob* _errorMsgs = nullptr;

	// Constant Buffer
	ID3D11Buffer* _matrixBuffer = nullptr;	// 정점도 buffer로 썻었지 쓰는 방식에 따라 달라져 
public:	//constructer and Deconstructer
	ColorShader(wstring shaderFileName);	//아예 쉐이더 파일 이름을 가지고 만들 것
	~ColorShader();
	// Getter
	ID3D11InputLayout* GetLayout() { return _inputLayout; }
public:	// public instance method
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation); 	// 인스턴스 드로우는 이걸로 그릴 것
private:	//private instance method
	void CreateShader(wstring shaderFileName);	// VS, PS 제작 
	void CreateInputLayout();	// 입력데이터 포맷
	void CreateMatrixBuffer();	// 행렬변환 안해서 0.5 기준으로 그려졌는데, Scale하기 위해 상수버퍼 만듬.
	bool CheckFile(wstring shaderFileName);
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
};