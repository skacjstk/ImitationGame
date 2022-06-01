#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
class S01_DXLine : public Scene
{
private:	//private instance variable
	struct Vertex	//DX에서 정점에 대한 데이터 Type을 정해주지 않아서, 직접 정의해서 씀
	{
		Vector3 Position;
		Color color;
	};
	struct TRANSFORM_DATA
	{
		Matrix World;	// 자기 자신
		Matrix View;	// Camera Position
		Matrix Projection;	// 직교투영 
	};

	// 정점 및 Shader 
	Vertex _vertices[10];
	ID3D11Buffer  *_vertexBuffer = nullptr; // 정점이 들어갈 버퍼
	ID3D11VertexShader *_vertexShader = nullptr; // Shader로 작성된 File을 interface 
	ID3D11PixelShader *_pixelShader = nullptr; // Shader로 작성된 File을 interface 
	ID3D10Blob	*_vsBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환
	ID3D10Blob	*_psBlob = nullptr;		//아까 적어놨던 HLSL 를 바이너리 로 변환
	ID3D11InputLayout* _inputLayout = nullptr;
	ID3D10Blob* _errorMsgs = nullptr;

	int _vertexCount = 0;
public:	//생 소
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
