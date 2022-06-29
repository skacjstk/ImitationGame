#include "ImitationGame/framework.h"
#include "AnimationClip.h"
#include "Render/Texture.h"

////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////
AnimationClip::AnimationClip(eState state)
{
	m_State = state;
}

AnimationClip::~AnimationClip()
{
	for (UINT i = 0; i < m_cvFrames.size(); i++)
		delete m_cvFrames[i];
	m_cvFrames.erase(m_cvFrames.begin(), m_cvFrames.end());

}
// ��ü �����غ� endXY ���� AddFrame ( �� �����ɸ� )
void AnimationClip::AddFrame(Texture * pTexture, wstring strImageFile, float startX, float startY, float delta)
{
	Frame *pFrame = new Frame();

	pFrame->m_strImageFile = strImageFile;	// �� IMAGE_FOLDER �� ������? ��¶�� �� �������� ������ Path ���� ��.
	pFrame->m_Offset = Vector2(startX, startY);
	pFrame->m_Time = delta;
	pTexture->CreateShaderResourceView(strImageFile); // �̸��� ���� ������ �� 1

	D3DX11_IMAGE_INFO ImageInfo;
	HRESULT hr = D3DX11GetImageInfoFromFile(strImageFile.c_str(), NULL, &ImageInfo, NULL);
	assert(SUCCEEDED(hr));

	pFrame->m_OffsetSize = Vector2(ImageInfo.Width, ImageInfo.Height);
	m_cvFrames.push_back(pFrame);
	m_ptrTexture = pTexture;
}
void AnimationClip::AddFrame(Texture * pTexture, wstring strImageFile, float startX, float startY, float endX, float endY, float delta)
{
	Frame *pFrame = new Frame();

	pFrame->m_strImageFile = strImageFile;	// �� IMAGE_FOLDER �� ������? ��¶�� �� �������� ������ Path ���� ��.
	pFrame->m_Offset = Vector2(startX, startY);
	pFrame->m_OffsetSize = Vector2(endX - startX, endY - startY);
	pFrame->m_Time = delta;
	pTexture->CreateShaderResourceView(strImageFile); // �̸��� ���� ������ �� 1
	m_cvFrames.push_back(pFrame);
	m_ptrTexture = pTexture;
}
void AnimationClip::AddFrame(wstring strImageFile, float startX, float startY, float endX, float endY, float delta)
{
	Frame *pFrame = new Frame();

	pFrame->m_strImageFile = strImageFile;
	pFrame->m_Offset = Vector2(startX, startY);
	pFrame->m_OffsetSize = Vector2(endX - startX, endY - startY);
	pFrame->m_Time = delta;
	if (m_ptrTexture == NULL)
	{
		MessageBox(NULL, L"Texture�� Null Pointer�Դϴ�", L"Game", NULL);
	}
	m_ptrTexture->CreateShaderResourceView(strImageFile); // �̸��� ���� ������ �� 1
	m_cvFrames.push_back(pFrame);

}
void AnimationClip::SetOffsetSize(float x, float y)
{
	for (UINT i = 0; i < m_cvFrames.size(); i++)
	{
		Frame *pFrame = m_cvFrames[i];
		pFrame->m_OffsetSize = Vector2(x, y);
	}


}
//////////////////////////////////////////////////////////////
// Animation Clip
/////////////////////////////////////////////////////////////
void AnimationClip::Update(Matrix V, Matrix P)
{
	// �̻�ó��
	if (m_cvFrames.size() == 0)   return;
	if(m_cvFrames.size() <= m_nCurrentFrame) return;

	Frame *pFrame = m_cvFrames[m_nCurrentFrame];  // image, deleta

	// �ð� Check
	if (IsPlay())
	{
		m_PlayTime = m_PlayTime + TIMEMANAGER->Delta();

		if (m_PlayTime >= pFrame->m_Time)
		{
			m_nCurrentFrame++;    // ���� Image Index
			if (m_nCurrentFrame == m_cvFrames.size())
			{
				if (m_State == eState::Once)  // ����
				{
					SetStop();
					m_nCurrentFrame = 0;
				}

				else if (m_State == eState::End)
				{
					SetStop();
					m_nCurrentFrame = (UINT)m_cvFrames.size() - 1;

				}
				else if (m_State == eState::EndStay)
				{
					m_nCurrentFrame = (UINT)m_cvFrames.size() - 1;

				}

				else	
				  m_nCurrentFrame = 0;
			}
			m_PlayTime = 0.0f;
		}
	}


	// Frame

	pFrame = m_cvFrames[m_nCurrentFrame];   // Offset, OffsetSize
	this->m_ptrTexture->SetImageFile(pFrame->m_strImageFile);
	this->m_ptrTexture->SetOffset(pFrame->m_Offset);
	this->m_ptrTexture->SetOffsetSize(pFrame->m_OffsetSize);
	this->m_ptrTexture->Update(V, P);
}

void AnimationClip::Render()
{
	// �̻�ó��
	if (m_cvFrames.size() == 0)   return;
	if (m_cvFrames.size() <= m_nCurrentFrame) return;

	this->m_ptrTexture->Render();
}
