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
	Vector2 cornerLeft = Vector2(0.0f, 0.0f);
	Vector2 cornerRight = Vector2(0.0f, 0.0f);

	Vector2 cameraBefore = Vector2(0.0f, 0.0f);
	Vector2 cameraAfter = Vector2(0.0f, 0.0f);

public:
	void   PreUpdate();  // Keyborad
	void   Update();     // View, Projection Matrix Update
	void   Update(Matrix V, Matrix P);
	void   Render();

	void   VCtoWC(Vector2& posotion);   // View   -> Window(2D��)
	void   AbsoluteVCtoWC(Vector2& position);	// ������ǥ View -> Window ��ȯ ( �ַ� UI ��)
	void   WCtoVC(Vector2& posotion);   // Window -> View

	// ���� ����
	void SetCornerLeft(float x, float y) { this->cornerLeft = Vector2(x, y); }
	void SetCornerRight(float x, float y) { this->cornerRight = Vector2(x, y); }
	void SetCornerLeft(Vector2 leftDown) { this->cornerLeft = leftDown; }
	void SetCornerRight(Vector2 rightTop) { this->cornerRight = rightTop; }


public:   // Setter
	void   SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void   SetPosition(Vector2 position) { m_Position = position; }
	void   SetObject(class GameObject* pObject) { m_pGameObject = pObject; }
	void   SetCenterXLock(int Value) { CenterXLock = Value; }
	void   SetOffset(float x, float y) { m_Offset = Vector2(x, y); }
public:   // Getter
	Matrix GetViewMatrix() { return m_View; }
	Matrix GetAbsoluteViewMatrix() { return m_abView; }
	Matrix GetProjectionMatrix() { return m_Projection; }
	Vector2 GetPosition() { return m_Position; }
	// Setter
	void SetShake(bool flag) { this->shake_ = flag; }
	void FadeIn(float duringTime = 0.5f, bool immediately = false);	// �긦 ȣ���ϸ� ���ο��� stdFunction �����Ͱ� �ٲ�
	void FadeOut(float duringTime = 0.5f, bool immediately = false);
private:
	void SetWaiting();
	void SetFadeIn();
	void SetFadeOut();
private:	//����� �Լ�
	void Shake(Vector2& position);
	std::function<void()> EffectUpdate;	// ī�޶� ��üȿ��
	std::function<void()> EffectRender;	// ī�޶� ��üȿ�� ( fade out, in ��)

	void FadeRender();
	void FadeInUpdate();
	void FadeOutUpdate();
	void WaitingUpdate() {};
	void WaitingRender() {};

private:
	Vector2 m_Position = Vector2(0.0f, 0.0f);
	Vector2 m_MoveSpeed = Vector2(500.0f, 500.0f);
	Matrix  m_Projection;
	Matrix  m_View;
	Matrix m_abView;
	class   Texture* m_pTexture = nullptr;
	class Texture* fadeTexture_ = nullptr;
	class   GameObject* m_pGameObject = nullptr;
	Vector2 m_Offset = Vector2(0.0f, 0.0f);
	int     CenterXLock = 1;
	// ī�޶� ����ũ ȿ��
	bool	shake_ = false;
	float time_ = 0.0f;
	// ī�޶� fade ȿ��
	float duringTime_ = 0.0f;
	float fadeAmount_ = 0.0f;	// �������� fade ��ȭ��
	float currentFade_ = 0.0f;
private:
	Camera();
public:
	~Camera();
};
