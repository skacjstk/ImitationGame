#include "ImitationGame/framework.h"
#include "Camera.h"
#include "Render/Texture.h"
#include "ImitationGame/MainWindow.h"
#include "Base/GameObject.h"

Camera::Camera()
{
	wstring imgFile = IMAGE_FOLDER;
	imgFile += L"Camera/Camera.png";
	wstring shaderFile = SHADER_FOLDER;
	shaderFile += L"Texture.hlsl";
	m_pTexture = new Texture(imgFile, shaderFile);

	// �⺻

	Vector3 eye = Vector3(0.0f, 0.0f, 0.0f);	// ���� ���� ��ġ
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 eye_at = eye + at;
	D3DXMatrixLookAtLH(&m_View, &eye, &eye_at, &up);


	//	D3DXMatrixOrthoLH(&m_Projection, (float)MAIN->GetWidth(),
	//		(float)MAIN->GetHeight(), 0, 1);

	D3DXMatrixOrthoOffCenterLH(&m_Projection,
		-(float)MAIN->GetWidth() * 0.5f,
		(float)MAIN->GetWidth() * 0.5f,
		-(float)MAIN->GetHeight() * 0.5f,
		(float)MAIN->GetHeight() * 0.5f,
		-1.0f, 1.0f);


}

Camera::~Camera()
{
	SAFE_DELETE(m_pTexture);

}

////////////////////////////////////////////////////
//  Keyboard�Է���, ī�޶� ����
////////////////////////////////////////////////////
void Camera::PreUpdate()
{
	Vector2 position = m_Position;
	if (KEYBOARD->Press(VK_LEFT))
	{
		position.x = position.x - TIMEMANAGER->Delta() * m_MoveSpeed.x;
	}
	if (KEYBOARD->Press(VK_RIGHT))
	{
		position.x = position.x + TIMEMANAGER->Delta() * m_MoveSpeed.x;
	}
	if (KEYBOARD->Press(VK_UP))
	{
		position.y = position.y - TIMEMANAGER->Delta() * m_MoveSpeed.y;
	}
	if (KEYBOARD->Press(VK_DOWN))
	{
		position.y = position.y + TIMEMANAGER->Delta() * m_MoveSpeed.y;
	}

	// Limit Check


	SetPosition(position);


}

void Camera::Update()
{
	if (!m_pGameObject)
		PreUpdate();
	else
	{
		Vector2 position = m_pGameObject->GetPosition();

		position = Vector2(position.x + m_Offset.x,
			position.y + m_Offset.y);
		if (CenterXLock == 1)
			position.x = 0.0f;
		/*
		if (position.x < cornerLeft.x)	// ��
			position.x = cornerLeft.x;
		else if (position.x > cornerRight.x)	// ��
			position.x = cornerRight.x;

		if (position.y < cornerLeft.y)	// �ϴ�
			position.y = cornerLeft.y;
		else if (position.y > cornerRight.y)	// ���
			position.y = cornerRight.y;
			*/
		SetPosition(position);
	}


	// �⺻

	Vector3 eye = Vector3(m_Position.x, m_Position.y, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 eye_at = eye + at;
	D3DXMatrixLookAtLH(&m_View, &eye, &eye_at, &up);

	// ������ǥ �� ��Ʈ���� ����
	Vector3 abEye = Vector3(0.0f, 0.0f, 0.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
	at = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 abEye_at = abEye + at;
	D3DXMatrixLookAtLH(&m_abView, &abEye, &abEye_at, &up);

	// Window�� ũ�� ����
	D3DXMatrixOrthoOffCenterLH(&m_Projection,
		-(float)MAIN->GetWidth() * 0.5f,
		(float)MAIN->GetWidth() * 0.5f,
		-(float)MAIN->GetHeight() * 0.5f,
		(float)MAIN->GetHeight() * 0.5f,
		-1.0f, 1.0f);

	//	Matrix S;
	//	D3DXMatrixScaling(&S, 0.7f,0.7f,0.0f);
	//
	//	m_Projection = m_Projection * S;

}

void Camera::Update(Matrix V, Matrix P)
{
	m_pTexture->Update(V, P);
}

void Camera::Render()
{
	m_pTexture->Render();
}
/////////////////////////////////////////////////////////
// View(������ǥ) Window��ǥ�� ��ȯ
// 2D�� Text, 2D Line ( DirectX:: �̿�)
////////////////////////////////////////////////////////
void Camera::VCtoWC(Vector2& posotion)
{
	Matrix vp = m_View * m_Projection;
	Matrix S, T, W;

	D3DXMatrixScaling(&S, MAIN->GetWidth() * 0.5f, -MAIN->GetHeight() * 0.5f, 0.0f);
	D3DXMatrixTranslation(&T, MAIN->GetWidth() * 0.5f, MAIN->GetHeight() * 0.5f, 0.0f);

	W = vp * S * T;
	D3DXVec2TransformCoord(&posotion, &posotion, &W);

}
void Camera::AbsoluteVCtoWC(Vector2& position)
{
	Vector3 eye = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 eye_at = eye + at;
	D3DXMatrixLookAtLH(&m_abView, &eye, &eye_at, &up);

	Matrix vp = m_abView * m_Projection;	// p ��Ʈ������ 2D���� ������ �ʴ´�.
	Matrix S, T, W;

	D3DXMatrixScaling(&S, MAIN->GetWidth() * 0.5f, -MAIN->GetHeight() * 0.5f, 0.0f);
	D3DXMatrixTranslation(&T, MAIN->GetWidth() * 0.5f, MAIN->GetHeight() * 0.5f, 0.0f);

	W = vp * S * T;
	D3DXVec2TransformCoord(&position, &position, &W);

}
/////////////////////////////////////////////////////
//   Window��ǥ�� View��ǥ
//   Mouse Position(������)
/////////////////////////////////////////////////////
void Camera::WCtoVC(Vector2& position)
{
	Matrix vp = m_View * m_Projection;
	// inverse matrix
	D3DXMatrixInverse(&vp, NULL, &vp);

	position.x = (position.x / MAIN->GetWidth()) * 2.0f - 1.0f;
	position.y = -((position.y / MAIN->GetHeight()) * 2.0f - 1.0f);

	D3DXVec2TransformCoord(&position, &position, &vp);
}
