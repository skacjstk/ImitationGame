#pragma once
class Line
{
public: 
	void     Update(Matrix V, Matrix P);
	void     Render();
	void     CreateVertexBuffer();
	void     ClearLine();
	void     ClearVertexBuffer() { SAFE_RELEASE(m_pVertexBuffer); }
	void     EndLine() { CreateVertexBuffer(); }
	int      GetCountLine() { return (int)(m_cvLines.size() / 2); }
	Vector2  GetStartPoint(int no);
	Vector2  GetEndPoint(int no);
	bool     LoadLine(string fileName);
	bool     SaveLine(string fileName);

public:
	void  AddLine(float x1, float y1, float x2, float y2);
	static bool IntersectionLine(Vector2 S1S, Vector2 S1E, Vector2 S2S, Vector2 S2E, Vector2 & I0, Vector2 & I1); //������ 
	static bool IntersectionLine(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2 & result); //������ 

private:
	struct Vertex
	{
		Vector3 Position;
		Color   Color;
	};

	vector<Vector2*>    m_cvLines;

	ID3D11Buffer		*m_pVertexBuffer = nullptr;   // c++����->ID3D11Buffer(Device)->pipeline(DeviceContext)
	ID3D11VertexShader  *m_pVertexShader = nullptr;   // Shader�� �ۼ��� ������ I/F
	ID3D11PixelShader   *m_pPixelShader = nullptr;    // Shader Pixel
	ID3D10Blob          *m_pVsBlob = nullptr;          // HLSL->Binary��ȯ
	ID3D10Blob          *m_pPsBlob = nullptr;          // HLSL->Binary��ȯ
	ID3D11InputLayout   *m_pInputLayout = nullptr;    // ���� --> Line,Rectangle,Triangle
													  // �Էµ����͸� ������� ó��?
	int   m_nVerexCount = 0;
	class  ColorShader  *m_pShader = nullptr;

public: // ������ & �Ҹ���
	Line();
	~Line();
};
