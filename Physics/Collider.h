#pragma once
////////////////////////////////////////////////
//  
//  GameObject의 Box형태  구현 Class
//  
////////////////////////////////////////////////
class Collider
{
public:
	struct SHAPE {
		Vector2 position;
		Vector2 scale;
		Vector2 min;
		Vector2 max;
		float   rot;
	};


public:
	struct Vertex   // DX에서는 정점에 대한 정의하지 않음, 사용작 정의, Class로 정의는 하지 않음
	{
		Vector3 Position;   // Color가 빠진 이유는 충돌이 일어 나면 빨강색
		Color   Color;
	};

public:
	void			Update(Matrix View, Matrix Projection);
	void			Render();
	
	// 
	static         bool   IntersectOBB(SHAPE A, SHAPE B);
	static         bool   IntersectOBB(class Collider *A, class Collider *B);
	static         bool   IntersectAABB(class Collider *A, class Collider *B);
	static         bool   InterSectionLine(Vector2 A, Vector2 B, Vector2 C, Vector2 D);
	static         bool   PointInRect(Matrix world, Vector2 pos);
	static         bool   Aabb(Matrix world, Vector2 pos);
	static         bool   InterSect(class Collider *A, class Collider *B);
	static         int    CCW(Vector2 p1, Vector2 p2, Vector2 p3);
	static         int    IsLeft(Vector2 left, Vector2 right);
	// Setter
	void		    SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void		    SetPosition(Vector2 pos) { m_Position = pos; }
	void		    SetScale(float x, float y) { m_Scale = Vector2(x, y); };
	void		    SetScale(Vector2 size) { m_Scale = size; };
	void		    SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); };
	void		    SetRotation(Vector3 rot) { m_Rotation = rot; };
	void            SetWorld(Matrix world) { m_World = world; }
	void            SetCollisionCheck(bool collision) { m_bCollisionCheck = collision; }
	
	// Getter
	bool				GetCollisionCheck() { return m_bCollisionCheck; }
	Vector2				GetPosition() { return m_Position; }
	Vector2				GetScale() { return m_Scale; }
	Vector3				GetRotation() { return m_Rotation; }
	Matrix				GetWorld() { return m_World; }
	vector<Vector2*>    GetAreas();
	static Vector2             GetUpVector(SHAPE s);
	static Vector2             GetRightVector(SHAPE s);
	static Vector2             GetUnitVector(Vector2 s);
private:
	void                CreateBuffer();
	void                UpdateBlock(Color color);   // Color값 변경


private:
	vector<Vector2*>   m_cvPolygons;
	Vector2			   m_Position = Vector2(0.0f, 0.0f);          // GameObject 좌표
	Vector2			   m_Scale = Vector2(1.0f, 1.0f);             // GameObject 크기
	Vector3			   m_Rotation = Vector3(0.0f, 0.0f, 0.0f);    // GameObject 방향 
	class  ColorShader  *m_pShader = nullptr;
	ID3D11Buffer       *m_pVertexBuffer = nullptr;
	Matrix             m_World;                                  // GameObject WorldMatrix  
	bool               m_bCollisionCheck = false;                // 충돌이 안되어 있는 경우는 false
public:
	Collider();
	~Collider();
};