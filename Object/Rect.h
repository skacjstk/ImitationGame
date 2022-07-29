#pragma once
class Rect
{
public:
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock(Color color);   // Color값 변경

public: // Setter
	void  SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void  SetPosition(Vector2 position) { m_Position = position; }
	void  SetScale(float sx, float sy) { m_Scale = Vector2(sx, sy); }
	void  SetScale(Vector2 scale) { m_Scale = scale; }
	void  SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void  SetRotation(Vector3 rotation) { m_Rotation = rotation; }
	void  SetColor(Color color) { m_Color = color; }

	Vector2  GetPosition() { return m_Position; }
	Vector2  GetScale() { return m_Scale; }
	Vector3  GetRotation() { return m_Rotation; }
	bool    IsPtInRect(Vector2 pos);


private:
	void    CreateVertexBuffer();
	void    CreateIndexBuffer();
	Vector2 m_Position = Vector2(0.0f, 0.0f);
	Vector2 m_Scale = Vector2(1.0f, 1.0f);
	Vector3 m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Color   m_Color = Color(1, 1, 0, 0);
	struct Vertex
	{
		Vector3 Position;
		Color   Color;
	};
	// 정점 밑 Shader
	ID3D11Buffer        *m_pVertexBuffer = nullptr;   // Device
	ID3D11Buffer        *m_pIndexBuffer = nullptr;
	class  ColorShader  *m_pShader = nullptr;
public:
	Rect(wstring strShaderFile);
	Rect();
	~Rect();
};

