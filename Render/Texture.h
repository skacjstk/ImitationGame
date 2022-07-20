#pragma once
class Texture
{
public:
	Vector3			_pivot = Vector3(0.0f, 0.0f, 0.0f);
private:	// General
	Vector2			_position		= Vector2(0.0f, 0.0f);
	Vector2			_scale			= Vector2(1.0f, 1.0f);
	Vector3			_rotation		= Vector3(0.0f, 0.0f, 0.0f);
	Vector2			_spriteOffset	= Vector2(0.0f, 0.0f);	// Render �� ���� ������
	Vector2			_spriteSize		= Vector2(0.0f, 0.0f);	// Render �� ���� size
	Vector2			_imageSize		= Vector2(0.0f, 0.0f);	// �� �̹��� ��üũ�� 
	wstring			_strImageFile	= L"";
	class TextureShader* _Shader = nullptr;
private:	// DirectX
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;		
	};
	ID3D11Buffer				*_vertexBuffer = nullptr; // ������ �� ����
	ID3D11ShaderResourceView	*_SRV = nullptr;	// Image������ Interface ���ִ� ����
public:
	Texture(wstring strImage, wstring strShader);
	Texture(wstring strImage, wstring strShader, int startX, int startY, int endX, int endY);
	~Texture();
private:
	Vector2 ConstructorCommonWork(wstring strImage);
public:
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock();	// Uv�� ����
public:	//setter
// ���ø� �Ẹ�� (strImageFile ���� ���� )
	void SetScale(float x, float y) { _scale = Vector2(x, y); }
	void SetScale(Vector2 scale) { _scale = scale; }
	void SetPosition(float x, float y) { _position = Vector2(x, y); }
	void SetPosition(Vector2 position) { _position = position; }
	void SetRotation(float x, float y, float z) { _rotation = Vector3(x, y, z); }
	void SetRotation(Vector3 rotation) { _rotation = rotation; }
	void ModifyPosition(Vector2 modifyPosition) { _position += modifyPosition; }
	void ModifyPosition(float x, float y) { _position += Vector2(x, y); }
	void SetOffset(float x, float y) { _spriteOffset = Vector2(x, y); }
	void SetOffset(Vector2 offset) { _spriteOffset = offset; }
	void SetOffsetSize(float x, float y) { _spriteSize = Vector2(x, y); }
	void SetOffsetSize(Vector2 spriteSize) { _spriteSize = spriteSize; }
	void SetImageFile(wstring strFile) { _strImageFile = strFile; }
	void SetImageSize(float x, float y) { _imageSize = Vector2(x, y); }
	void CreateShaderResourceView(wstring strFile);
	void CreateMultiShaderResourceView(wstring strFile);
public: // Getter
	Vector2 GetPosition() { return _position; }
	Vector2 GetScale() { return _scale; }
	Vector3 GetRotation() { return _rotation; }
	Vector2 GetOffset() { return _spriteOffset; }
	Vector2 GetOffsetSize() { return _spriteSize; }
	wstring GetImageFile() { return _strImageFile; }
	Vector2 GetImageSize() { return _imageSize; }
	Vector2 GetSpriteSize() { return _spriteSize; }
	Vector2 GetTextureRealSize() { return Vector2(_scale.x * _spriteSize.x, _scale.y * _spriteSize.y); }
	// effect ���� �̿��� ������ 
	void UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3);
	void UpdateColorBuffer(int path);
	void UpdateColorBuffer(Color color, int path);
	void UpdateColorBuffer(Color color, int path, float Time);
private:
	void CreateVertexBuffer(float startX, float startY, float endX, float endY);
	TextureShader* InputShader(wstring strShader);
};
