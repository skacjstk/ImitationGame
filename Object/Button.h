#pragma once
class Button : public UIObject
{
private:
	Color	_Default = Color(1.0f,1.0f,1.0f,1.0f);
	Color	_Hover = Color(201.0f / 255.0f, 82.0f / 255.0f, 80.0f / 255.0f, 1.0f);
	wstring _strButtonName = L"";	// 버튼의 텍스트 내용물 ( 새 게임, 챕터 선택, 나가기 등)
	Texture* _button[2];	// 버튼의 이미지 Texture ( 일반, 호버링 )
	float _fontSize = 30.0f;
	int _shaderInputType = 1;
public:
	Button(wstring strImage, wstring strHoverImage, wstring strShader);
	~Button();
public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void InputUpdate();

	// Setter
	void SetButtonName(wstring strName) { _strButtonName = strName; }
	void SetHoverColor(Color color) { _Hover = color; }
	void SetDefaultColor(Color color) { _Default = color; }
	void SetFontSize(float size) { _fontSize = size; }
	void SetShaderInputType(int id) { _shaderInputType = id; }

	// Getter
	wstring GetButtonName() { return _strButtonName; }
};
