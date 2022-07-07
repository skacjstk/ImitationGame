#pragma once
class Texture
{
public:
	Vector3			_pivot = Vector3(0.0f, 0.0f, 0.0f);
private:	// General
	Vector2			_position		= Vector2(0.0f, 0.0f);
	Vector2			_scale			= Vector2(1.0f, 1.0f);
	Vector3			_rotation		= Vector3(0.0f, 0.0f, 0.0f);
	Vector2			_spriteOffset	= Vector2(0.0f, 0.0f);	// Render 될 영역 시작점
	Vector2			_spriteSize		= Vector2(0.0f, 0.0f);	// Render 될 영역 size
	Vector2			_imageSize		= Vector2(0.0f, 0.0f);	// 한 이미지 전체크기 
	wstring			_strImageFile	= L"";
	class TextureShader* _Shader = nullptr;
private:	// DirectX
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;		
	};
	ID3D11Buffer				*_vertexBuffer = nullptr; // 정점이 들어갈 버퍼
	ID3D11ShaderResourceView	*_SRV = nullptr;	// Image파일을 Interface 해주는 역할
public:
	Texture(wstring strImage, wstring strShader);
	Texture(wstring strImage, wstring strShader, int startX, int startY, int endX, int endY);
	~Texture();
private:
	Vector2 ConstructorCommonWork(wstring strImage);
public:
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock();	// Uv값 변경
public:	//setter
// 템플릿 써보자 (strImageFile 제외 전부 )
	void SetScale(float x, float y) { _scale = Vector2(x, y); }
	void SetScale(Vector2 scale) { _scale = scale; }
	void SetPosition(float x, float y) { _position = Vector2(x, y); }
	void SetPosition(Vector2 position) { _position = position; }
	void SetRotation(float x, float y, float z) { _rotation = Vector3(x, y, z); }
	void SetRotation(Vector3 rotation) { _rotation = rotation; }
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
	// effect 사용시 이용할 데이터 
	void UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3);
	void UpdateColorBuffer(int path);
	void UpdateColorBuffer(Color color, int path);
	void UpdateColorBuffer(Color color, int path, float Time);
private:
	void CreateVertexBuffer(float startX, float startY, float endX, float endY);
	TextureShader* InputShader(wstring strShader);
};
