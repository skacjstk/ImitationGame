#include "ImitationGame/framework.h"
#include "Button.h"

Button::Button(wstring strImage, wstring strHoverImage, wstring strShader)
{
	_button[0] = new Texture(strImage, strShader);
	_button[1] = new Texture(strHoverImage, strShader);
}

Button::~Button()
{
	SAFE_DELETE(_button[0]);
	SAFE_DELETE(_button[1]);
}

void Button::Update(Matrix V, Matrix P)
{
	InputUpdate();
	for (int i = 0; i < sizeof(_button) / sizeof(_button[0]); ++i){
		_button[i]->SetPosition(GetPosition());
		_button[i]->SetScale(GetScale());
		_button[i]->UpdateColorBuffer(_Default, _shaderInputType);
	}
	_button[1]->UpdateColorBuffer(_Hover, _shaderInputType);

	_button[0]->Update(V,P);
	_button[1]->Update(V,P);
}

void Button::Render()
{
	if(!_activate)
		_button[0]->Render();
	else
		_button[1]->Render();
	DirectWrite::BeginDraw();
	{
		float x = GetPosition().x-(_strButtonName.length() * _fontSize * 0.5f);	// font size , �߰������� ����ġ
		float y = GetPosition().y + (_fontSize * 0.65f);	// ������ġ�� 18, �������� 0.6f
		Vector2 pos = Vector2(x, y);
		CAMERA->VCtoWC(pos);
		DirectWrite::RenderText(_strButtonName, pos, 255, 255, 255, _fontSize);
	}
	DirectWrite::EndDraw();
}

void Button::Reset()
{
}

void Button::InputUpdate()
{
	// Ű����, Check
	if (this->_activate) {
		if (KEYBOARD->Down(VK_RETURN) || KEYBOARD->Down(VK_SPACE)) {
			if (_fpEnterCallback)
				_fpEnterCallback(this->_ID);
		}

		if (KEYBOARD->Down('W') || KEYBOARD->Down(VK_UP)) {
			if (this->tabGroup[2]) {
				this->SetActivate(false);
				tabGroup[2]->SetActivate(true);
			}
		}
		if (KEYBOARD->Down('S') || KEYBOARD->Down(VK_DOWN)) {
			if (this->tabGroup[3]) {
				this->SetActivate(false);
				tabGroup[3]->SetActivate(true);
			}
		}
		if (KEYBOARD->Down('A') || KEYBOARD->Down(VK_LEFT)) {
			if (this->tabGroup[0]) {
				this->SetActivate(false);
				tabGroup[0]->SetActivate(true);
			}
		}
		if (KEYBOARD->Down('D') || KEYBOARD->Down(VK_RIGHT)) {
			if (this->tabGroup[1]) {
				this->SetActivate(false);
				tabGroup[1]->SetActivate(true);
			}
		}
	}
}
