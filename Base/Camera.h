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

	void   VCtoWC(Vector2& posotion);   // View   -> Window(2D용)
	void   AbsoluteVCtoWC(Vector2& position);	// 절대좌표 View -> Window 변환 ( 주로 UI 용)
	void   WCtoVC(Vector2& posotion);   // Window -> View

	// 범위 지정
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
	void FadeIn(float duringTime = 0.5f, bool immediately = false);	// 얘를 호출하면 내부에서 stdFunction 포인터가 바뀜
	void FadeOut(float duringTime = 0.5f, bool immediately = false);
private:
	void SetWaiting();
	void SetFadeIn();
	void SetFadeOut();
private:	//비공개 함수
	void Shake(Vector2& position);
	std::function<void()> EffectUpdate;	// 카메라 전체효과
	std::function<void()> EffectRender;	// 카메라 전체효과 ( fade out, in 등)

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
	// 카메라 쉐이크 효과
	bool	shake_ = false;
	float time_ = 0.0f;
	// 카메라 fade 효과
	float duringTime_ = 0.0f;
	float fadeAmount_ = 0.0f;	// 실질적인 fade 변화량
	float currentFade_ = 0.0f;
private:
	Camera();
public:
	~Camera();
};
