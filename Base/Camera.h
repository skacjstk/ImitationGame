#pragma once
#define   CAMERA Camera::GetInstance()
class Camera
{
public:
	static Camera* GetInstance()
	{
		static Camera singleToneInstance;
		return &singleToneInstance;
	}

public:
	void   PreUpdate();  // Keyborad
	void   Update();     // View, Projection Matrix Update
	void   Update(Matrix V, Matrix P);
	void   Render();

	void   VCtoWC(Vector2& posotion);   // View   -> Window(2D¿ë)
	void   WCtoVC(Vector2& posotion);   // Window -> View



public:   // Setter
	void   SetPosition(float x, float y)  { m_Position = Vector2(x, y); }
	void   SetPosition(Vector2 position)  { m_Position = position; }
	void   SetObject(class GameObject *pObject) { m_pGameObject = pObject; }
public:   // Getter
	Matrix GetViewMatrix() { return m_View; }
	Matrix GetProjectionMatrix() { return m_Projection; }
	Vector2 GetPosition() { return m_Position; }



private:
	Vector2 m_Position  = Vector2(0.0f, 0.0f);
	Vector2 m_MoveSpeed = Vector2(200.0f, 200.0f);
	Matrix  m_Projection;
	Matrix  m_View;
	class   Texture     *m_pTexture  = nullptr;
	class   GameObject  *m_pGameObject    = nullptr;
	Vector2 m_Offset = Vector2(0.0f, 0.0f);
	int     CenterXLock = 1;

private:
	Camera();
public:
	~Camera();
};
