#pragma once
class ColorShader
{
private:	// private instance variable
	struct TRANSFORM_DATA
	{
		Matrix World;	// �ڱ� �ڽ� Scale, Position, Rotate�� �� �� �ִ�. �׷� ��� ������ 
		Matrix View;	// Camera Position
		Matrix Projection;	// �������� (�翵)
	};
	//Buffer�� Scene���� �޶� Scene �������� ���� 
	ID3D11VertexShader* _vertexShader = nullptr; // Shader�� �ۼ��� File�� interface		//VS
	ID3D11PixelShader* _pixelShader = nullptr; // Shader�� �ۼ��� File�� interface		//PS
	ID3D10Blob* _vsBlob = nullptr;		//�Ʊ� ������� HLSL �� ���̳ʸ� �� ��ȯ	//VS�� ��
	ID3D10Blob* _psBlob = nullptr;		//�Ʊ� ������� HLSL �� ���̳ʸ� �� ��ȯ	//PS�� ��
	ID3D11InputLayout* _inputLayout = nullptr;		//IA	���� --> Line,Rectangle,Triangle
	ID3D10Blob* _errorMsgs = nullptr;

	// Constant Buffer
	ID3D11Buffer* _matrixBuffer = nullptr;	// ������ buffer�� ������ ���� ��Ŀ� ���� �޶��� 
public:	//constructer and Deconstructer
	ColorShader(wstring shaderFileName);	//�ƿ� ���̴� ���� �̸��� ������ ���� ��
	~ColorShader();
	// Getter
	ID3D11InputLayout* GetLayout() { return _inputLayout; }
public:	// public instance method
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT VertexCount, UINT StartVertexLocation, int BaseVertexLocation); 	// �ν��Ͻ� ��ο�� �̰ɷ� �׸� ��
private:	//private instance method
	void CreateShader(wstring shaderFileName);	// VS, PS ���� 
	void CreateInputLayout();	// �Էµ����� ����
	void CreateMatrixBuffer();	// ��ĺ�ȯ ���ؼ� 0.5 �������� �׷����µ�, Scale�ϱ� ���� ������� ����.
	bool CheckFile(wstring shaderFileName);
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
};