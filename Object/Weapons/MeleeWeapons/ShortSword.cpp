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
	// 기본 배율은 6배
	SetWeaponScale(6.0f * WSCALEY, 6.0f * WSCALEY);
}

ShortSword::~ShortSword()
{
}

void ShortSword::Update(Matrix V, Matrix P)
{
	// AnimationUpdate
	weapon_->SetPlay(0);

	// 여기서 실질적으로 휘두르는 애니메이션을 관리하게 할 것. (마우스 위치점 기반.. 등등)

	weapon_->SetPosition(GetWeaponPosition());
	weapon_->SetRotation(GetWeaponRotation());
	weapon_->SetScale(GetWeaponScale());
	weapon_->Update(V, P);
}

void ShortSword::Render()
{
	weapon_->Render();
}

void ShortSword::Reset()
{
}

void ShortSword::Fire()
{
	// 무기의 공격 
	// 무기 애니메이션의 위치값 조정 
}
