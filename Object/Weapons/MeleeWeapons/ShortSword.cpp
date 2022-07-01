#include "ImitationGame/framework.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "ShortSword.h"

ShortSword::ShortSword()
{
	minDamage_ = 8;
	maxDamage_ = 10;
	ItemText_ = "가볍고 휘두르기 편한 검";
	handedType_ = Weapon::HandedType::ONEHANDED;
	if (static_cast<int>(handedType_) == 1)
		printf("static cast 한손무기\n");
	if (handedType_ == Weapon::HandedType::ONEHANDED)
			printf("한손무기\n");
	printf("무기타입: %d\n", static_cast<int>(weaponType_));
	attackDelay_ = 1 / attackSpeed_;	// 예를 들어, speed가2 면 attackdelay는 0.5가 되어. 0.5초마다 1번씩 공격하는거임.

	wstring strImage = IMAGE_FOLDER;
	strImage += L"Weapon/OneHanded/Melee/ShortSword/ShortSword.png";
	wstring 	strShader = SHADER_FOLDER;
	strShader += L"Texture.hlsl";
	frontImage_ = new Texture(strImage, strShader);

	weapon_ = new Animation(strImage, strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		pClip->AddFrame(weapon_->GetTexture(), strImage, 0, 0, 0.1f);
		weapon_->AddClip(pClip);
	}
	
}

ShortSword::~ShortSword()
{
}

void ShortSword::Update(Matrix V, Matrix P)
{
	weapon_->SetPlay(0);
	weapon_->SetPosition(0.0f, 0.0f);
	weapon_->SetRotation(0.0f, 0.0f, 0.0f);
	weapon_->SetScale(1.0f, 1.0f);
	weapon_->Update(V, P);
}

void ShortSword::Render()
{
	weapon_->Render();
}

void ShortSword::Reset()
{
}
