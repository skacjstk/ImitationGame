#pragma once
class Line
{
public:
	Vector2 lineScale_ = Vector2(1.0f, 1.0f);
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
	void     EraseLine(Vector2 pos, float gap = 1.0f);    // 마우스가 Click된 위치
	void     SetColor(Color color) { m_Color = color; }	

public:
	void         AddLine(float x1, float y1, float x2, float y2);
	static bool  IntersectionLine(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2 & result); //교차점 
	static bool  Clipping(Vector2 & start, Vector2 & end, Vector2 AreaMin, Vector2 AreaMax);
	static bool  Clipping(Vector2 start, Vector2 end, class Collider* pCollider);	// Clipping 기반으로
//  static bool  IntersectionColliderToLine(class Collider* pCollider,  Vector2& start, Vector2& end);
	static float GetDistancePointToline(LONG& x, LONG& y, FLOAT& l, FLOAT& t, FLOAT& r, FLOAT& b);
private:
	static  int  IsLeft(Vector2 left, Vector2 right);
	static  bool CheckInterSectionLine(Vector2 A, Vector2 B, Vector2 C, Vector2 D);
	static 	int  CCW(Vector2 p1, Vector2 p2, Vector2 p3);
	static int   OutCode(Vector2 pt, Vector2 AreaMin, Vector2 AreaMax);


private:
	struct Vertex
	{
		Vector3 Position;
		Color   Color;
	};
	vector<Vector2*>    m_cvLines;

	ID3D11Buffer		*m_pVertexBuffer = nullptr;   // c++정점->ID3D11Buffer(Device)->pipeline(DeviceContext)
	ID3D11VertexShader  *m_pVertexShader = nullptr;   // Shader로 작성된 파일을 I/F
	ID3D11PixelShader   *m_pPixelShader = nullptr;    // Shader Pixel
	ID3D10Blob          *m_pVsBlob = nullptr;          // HLSL->Binary변환
	ID3D10Blob          *m_pPsBlob = nullptr;          // HLSL->Binary변환
	ID3D11InputLayout   *m_pInputLayout = nullptr;    // 정점 --> Line,Rectangle,Triangle
													  // 입력데이터를 어떤식으로 처리?
	int   m_nVerexCount = 0;
	class  ColorShader  *m_pShader = nullptr;
	Color   m_Color = Color(1.0f, 1.0f, 0.0f, 0.0f);

public: // 생성자 & 소멸자
	Line();
	~Line();
};
