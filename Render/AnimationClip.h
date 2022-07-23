#pragma once
/////////////////////////////////////////////
//  Animation
//    AnimationClip
//      Texture
////////////////////////////////////////////
class AnimationClip
{
public:
	enum eState
	{
		Loop = 0,     // IDLE
		Once,         // Attack   1번째 그림
		End,          // Attack   end번째 그림
		EndStay
	};
	void  Update(Matrix V, Matrix P); 
	void  Render();
public: // Getter
	bool  IsPlay()                     { return m_bPlay; }
	UINT GetCurrentFrame()			   { return m_nCurrentFrame; }
	Vector2 GetCurrentFrameRealSize()  { return m_ptrTexture->GetTextureRealSize(); }	// 0622 신규추가, 로딩 애니메이션 용
	Vector3 GetClipPivot()			   { return m_ClipPivot; }
public: // Setter	Texture 에서 템플릿 써보도록 하자 
	void  AddFrame(Texture *pTexture, wstring strImageFile, float startX, float startY, float endX, float endY, float delta);
	void  AddFrame(wstring strImageFile, float startX, float startY, float endX, float endY, float delta);
	void  AddFrame(Texture * pTexture, wstring strImageFile, float startX, float startY, float delta, int pivotOPT = 0x00); //자체제작
	void  AddFrame(Texture * pTexture, wstring strImageFile, float startX, float startY, float endY, float delta); //자체제작
	void  AddFrame(class Texture* pTexture, wstring strImageFile, float startX, float startY, float endX, float endY, float delta, float pivX, float pivY);	// pivot XY 추가본 : 시점: BWS
	void  SetPosition(float x, float y) { m_ptrTexture->SetPosition(x, y); }
//	void  SetPosition(Vector2 position) { m_ptrTexture->SetPosition(position); }
	void  SetScale(float sx, float sy)  { m_ptrTexture->SetScale(sx, sy); }
//	void  SetScale(Vector2 scale)       { m_ptrTexture->SetScale(scale); }
	void  SetRotation(float x, float y, float z) { m_ptrTexture->SetRotation(x, y, z); }
	void  SetStop()                     { m_bPlay = false; }
	void  SetTexture(class Texture *pTexture) { m_ptrTexture = pTexture; }
	void  SetPlay()   { m_nCurrentFrame = 0; m_bPlay = true; m_PlayTime = 0.0f;}
	void  SetClipPivot(Vector3 pivot) { m_ClipPivot = pivot; }
	void  SetOffsetSize(float x, float y);
private:
	struct Frame
	{
		wstring  m_strImageFile;
		Vector2  m_Offset     = Vector2(0.0f, 0.0f);  // Texture UpdateBlock()
		Vector2  m_OffsetSize = Vector2(0.0f, 0.0f);  // Texture UpdateBlock()
		Vector3  m_framePivot = Vector3(0.0f, 0.0f, 0.0f);
		float    m_Time = 0.0f;  // 동작시간
	};
	vector<Frame*>   m_cvFrames; 
	class  Texture  *m_ptrTexture = nullptr;   // 여기서 생성하지 않음 ( 여기 저장되는건 맞음 )
	Vector3 m_ClipPivot = Vector3(0.0f, 0.0f, 0.0f);
	eState  m_State = eState::Loop;
	UINT    m_nCurrentFrame = 0;  
	float   m_PlayTime = 0.0f;
	bool    m_bPlay = true;        // Animation이 Playe중일때는 , Attack동작이 끝나면 false
public:
	AnimationClip(eState state= eState::Loop);
	~AnimationClip();
};
