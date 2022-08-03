#include "ImitationGame/framework.h"
#include "ObjectPool.h"

Animation** ObjectPool::GetSlashEffect()
{
	for (UINT i = 0; i < slashEffectPool_.size(); ++i) {
		if (slashEffectPool_[i]->IsPlay() == false)
			return &slashEffectPool_[i];
	}
	// 여기까지 온거면 리턴 못받은거임. 그러니 하나 더 만들어서 끝자락에 push_back 된걸 보내줌
	AddSlashEffect();	
	return &slashEffectPool_[slashEffectPool_.size() - 1];
}

Animation** ObjectPool::GetMonsterDieEffect()
{
	for (UINT i = 0; i < monsterDieEffectPool_.size(); ++i) {
		if (monsterDieEffectPool_[i]->IsPlay() == false)
			return &monsterDieEffectPool_[i];
	}
	// 여기까지 온거면 리턴 못받은거임. 그러니 하나 더 만들어서 끝자락에 push_back 된걸 보내줌
	AddMonsterDieEffect();
	return &monsterDieEffectPool_[monsterDieEffectPool_.size() - 1];
}

void ObjectPool::AddSlashEffect()
{
	wstring strShader = L"TextureColor.hlsl";
	// SlashEffect 대입
	wstring strImage = L"FX/SlashFX/SlashFX0.png";
	Animation* slashEffect_ = new Animation(IMAGE_FOLDER + strImage, SHADER_FOLDER + strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		for (int i = 0; i < 4; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"FX/SlashFX/SlashFX" + to_wstring(i) + L".png";
			pClip->AddFrame(slashEffect_->GetTexture(), strImage, 0, 0, 42, 5, 0.07f);
		}
		slashEffect_->AddClip(pClip);
		pClip->SetStop();
	}
	slashEffectPool_.push_back(slashEffect_);
}

void ObjectPool::AddMonsterDieEffect()
{
	wstring strShader = L"TextureColor.hlsl";
	// SlashEffect 대입
	wstring strImage = L"FX/DieFX/Die01.png";
	Animation* dieEffect_ = new Animation(IMAGE_FOLDER + strImage, SHADER_FOLDER + strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		for (int i = 1; i <= 10; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"FX/DieFX/Die" + to_wstring(i / 10) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(dieEffect_->GetTexture(), strImage, 0, 0, 0.06f);
		}
		dieEffect_->AddClip(pClip);
		pClip->SetStop();
	}
	monsterDieEffectPool_.push_back(dieEffect_);
}

ObjectPool::ObjectPool()
{
	// 3개 정도 미리 만들어두기
	for (int i = 0; i < 3; ++i) {
		AddMonsterDieEffect();
		AddSlashEffect();
	}
}

ObjectPool::~ObjectPool()
{
	for (int i = 0; i < slashEffectPool_.size(); ++i) {
		SAFE_DELETE(slashEffectPool_[i]);
	}
	for (int i = 0; i < monsterDieEffectPool_.size(); ++i) {
		SAFE_DELETE(monsterDieEffectPool_[i]);
	}
}
