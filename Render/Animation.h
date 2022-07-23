#pragma once
class Animation
{
public:
	void Update(Matrix V, Matrix P);
	void Render();

public: // Setter
	void  SetOffsetSize(float x, float y);
	void  SetPosition(float x, float y)          { _texture->SetPosition(x, y); }
	void  SetPosition(Vector2 position)          { _texture->SetPosition(position); }
	void  SetScale(float sx, float sy)           { _texture->SetScale(sx, sy); }
	void  SetScale(Vector2 scale)                { _texture->SetScale(scale); }
	void  SetPivot(Vector3 pivot)				 { _texture->_pivot = pivot; }
	void  SetRotation(float x, float y, float z) { _texture->SetRotation(x, y, z); }
	void  SetRotation(Vector3 rotation)          { _texture->SetRotation(rotation); }
	void  ModifyPosition(Vector2 modifyPosition) { _texture->ModifyPosition(modifyPosition); }
	void  ModifyPosition(float x, float y)		 { _texture->ModifyPosition(Vector2(x, y)); }
	void  AddClip(class AnimationClip *pClip);
	void  SetPlay(UINT PlayNo, bool IsInitialize = false);                 // AnimationClip�� index�� ����
	void  SetStop();                            // ���������� Stop�ϰ� ������

public: // Getter

	bool  IsPlay();                            // ���� Aniamtion�� ���۵ǰ� �ִ°�?
	class Texture* GetTexture()                   { return _texture; }
	Vector2  GetPosition()                        { return _texture->GetPosition(); }
	Vector2  GetScale()                           { return _texture->GetScale(); }
	Vector3  GetRotation()                        { return _texture->GetRotation(); }
	Vector2  GetTextureRealSize()                 { return _texture->GetTextureRealSize(); }
	Vector3  GetPivot()							  { return _texture->_pivot; }
	UINT     GetClipNo()                          { return m_nCurrentClip; }
	AnimationClip* GetAnimationClip(int clipNo)   { return m_cvAnimationClips[clipNo]; }
private:
	class Texture *_texture = nullptr;
	vector<class AnimationClip*> m_cvAnimationClips;
	UINT  m_nCurrentClip = 0;

public:
	Animation(wstring imgFile, wstring shaderFile);
	~Animation();
};
