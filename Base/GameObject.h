#pragma once
class GameObject
{
public:
	enum Direction   // Left, up , Right, Down
	{
		Left = 0,
		Right,
		Up,
		Down
	};
public:
	virtual void Update(Matrix V, Matrix P) = 0;  // 순수가상함수
	virtual void Render() = 0;                    // 순수가상함수
	virtual void Reset()  = 0;                    // 순수가상함수

public: // Setter
	void  SetPosition(float x, float y)          { _position = Vector2(x, y); }
	void  SetPosition(Vector2 position)          { _position = position; }
	void  SetY(float Y) { _position.y = Y; }
	void  ModifyPosition(Vector2 modifyPosition) { _position += modifyPosition; }
	void  ModifyPosition(float x, float y)		 { _position += Vector2(x,y); }
	void  SetScale(float sx, float sy)           { _scale = Vector2(sx, sy); }
	void  SetScale(Vector2 scale)                { _scale = scale; }
	void  SetRotation(float x, float y, float z) { _rotation = Vector3(x, y, z); }
	void  SetActive(bool value)                  { _bActive = value; }
public: // Getter
	Vector2  GetPosition()                       { return _position; }
	Vector2  GetScale()                          { return _scale; }
	Vector3  GetRotation()                       { return _rotation; }
	bool     IsActive()							 { return _bActive; }
protected:
	Vector2  _scale    = Vector2(1.0f, 1.0f);
	Vector2  _position = Vector2(0.0f, 0.0f);
	Vector3  _rotation = Vector3(0.0f, 0.0f, 0.0f);
	string   _strName;                    // Object Name
	bool     _bActive = true;             // Object 활성화/비활성화
	bool     _bCollisionCheck = false;    // 충돌 Check

public:
	GameObject() {};
	~GameObject(){};

};
