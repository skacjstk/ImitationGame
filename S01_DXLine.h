#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
class S01_DXLine : public Scene
{
private:	//private instance variable
	struct Vertex	//DX���� ������ ���� ������ Type�� �������� �ʾƼ�, ���� �����ؼ� ��
	{
		Vector3 Position;
		Color color;
	};
	struct TRANSFORM_DATA
	{
		Matrix World;	// �ڱ� �ڽ�
		Matrix View;	// Camera Position
		Matrix Projection;	// �������� 
	};

	// ���� �� Shader 
	Vertex _vertices[10];
	ID3D11Buffer  *_vertexBuffer = nullptr; // ������ �� ����
	ID3D11VertexShader *_vertexShader = nullptr; // Shader�� �ۼ��� File�� interface 
	ID3D11PixelShader *_pixelShader = nullptr; // Shader�� �ۼ��� File�� interface 
	ID3D10Blob	*_vsBlob = nullptr;		//�Ʊ� ������� HLSL �� ���̳ʸ� �� ��ȯ
	ID3D10Blob	*_psBlob = nullptr;		//�Ʊ� ������� HLSL �� ���̳ʸ� �� ��ȯ
	ID3D11InputLayout* _inputLayout = nullptr;
	ID3D10Blob* _errorMsgs = nullptr;

	int _vertexCount = 0;
public:	//�� ��
	S01_DXLine();
	~S01_DXLine();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;

public:	//public instance method
private:	//private instance method
	void CreateVertexBuffer();
	void CreateShader();
	void CreateInputLayout();
	void OutputErrorMessage(ID3D10Blob* errorMsgs);
};
