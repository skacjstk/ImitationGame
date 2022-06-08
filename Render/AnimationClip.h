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
		Once,         // Attack   1��° �׸�
		End,          // Attack   end��° �׸�
		EndStay
	};
	void  Update(Matrix V, Matrix P); 
	void  Render();
public: // Getter
	bool  IsPlay()                     { return m_bPlay; }

public: // Setter	Texture ���� ���ø� �Ẹ���� ���� 
	void  AddFrame(class Texture *pTexture, wstring strImageFile, float startX, float startY, float endX, float endY, float delta);
	void  AddFrame(wstring strImageFile, float startX, float startY, float endX, float endY, float delta);
	void  SetPosition(float x, float y) { m_ptrTexture->SetPosition(x, y); }
//	void  SetPosition(Vector2 position) { m_ptrTexture->SetPosition(position); }
	void  SetScale(float sx, float sy)  { m_ptrTexture->SetScale(sx, sy); }
//	void  SetScale(Vector2 scale)       { m_ptrTexture->SetScale(scale); }
	void  SetRotation(float x, float y, float z) { m_ptrTexture->SetRotation(x, y, z); }
	void  SetStop()                     { m_bPlay = false; }
	void  SetTexture(class Texture *pTexture) { m_ptrTexture = pTexture; }
	void  SetPlay()   { m_nCurrentFrame = 0; m_bPlay = true; m_PlayTime = 0.0f;}
	void  SetOffsetSize(float x, float y);
private:
	struct Frame
	{
		wstring  m_strImageFile;
		Vector2  m_Offset     = Vector2(0.0f, 0.0f);  // Texture UpdateBlock()
		Vector2  m_OffsetSize = Vector2(0.0f, 0.0f);  // Texture UpdateBlock()
		float    m_Time = 0.0f;  // ���۽ð�
	};

	vector<Frame*>   m_cvFrames; 
	class  Texture  *m_ptrTexture = nullptr;   // ���⼭ �������� ����
	eState  m_State = eState::Loop;
	UINT    m_nCurrentFrame = 0;  
	float   m_PlayTime = 0.0f;
	bool    m_bPlay = true;        // Animation�� Playe���϶��� , Attack������ ������ false
public:
	AnimationClip(eState state= eState::Loop);
	~AnimationClip();
};
