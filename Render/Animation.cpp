#include "ImitationGame/framework.h"
#include "Animation.h"
#include "Render/AnimationClip.h"
#include "Render/Texture.h"
//////////////////////////////////////////////////////////
// ������
/////////////////////////////////////////////////////////
Animation::Animation(wstring imgFile, wstring shaderFile)
{
	_texture = new Texture(imgFile, shaderFile);
}

Animation::~Animation()
{
	SAFE_DELETE(_texture);

	for (UINT i = 0; i < m_cvAnimationClips.size(); i++)
		delete m_cvAnimationClips[i];
	m_cvAnimationClips.erase(m_cvAnimationClips.begin(), m_cvAnimationClips.end());

}
void Animation::AddClip(AnimationClip* pClip)
{
	pClip->SetTexture(this->_texture);
	m_cvAnimationClips.push_back(pClip);
}

//////////////////////////////////////////////////////////////////
//  Player�� Update()���� ������ Call
//////////////////////////////////////////////////////////////////
void Animation::SetPlay(UINT PlayNo, bool IsInitialize)
{
	// ������ Play�� Number�� �ٲ��� �ʰ� �� �ʿ䰡 ����

	AnimationClip* pClip = nullptr;
	// �̻�ó��
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];

	if (!IsInitialize) {
		if ((pClip->IsPlay() && PlayNo == m_nCurrentClip))
			return;
	}
	
	m_nCurrentClip = PlayNo;
	pClip->SetPlay();    // PlayTime, Image�� ������ 0�� �ٲپ� ���´�
}

void Animation::SetStop()
{
	// �̻�ó��
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	AnimationClip* pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->SetStop();
}

bool Animation::IsPlay()
{
	AnimationClip* pClip = m_cvAnimationClips[m_nCurrentClip];
	return pClip->IsPlay();
}

void Animation::Update(Matrix V, Matrix P)
{
	// �̻�ó��
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	AnimationClip* pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->Update(V, P);

}

void Animation::Render()
{
	// �̻�ó��
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	AnimationClip* pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->Render();
}

void Animation::SetOffsetSize(float x, float y)
{
	for (UINT i = 0; i < m_cvAnimationClips.size(); i++)
	{
		AnimationClip* pClip = m_cvAnimationClips[i];
		pClip->SetOffsetSize(x, y);
	}
}