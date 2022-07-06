#include "ImitationGame/framework.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "ShortSword.h"

ShortSword::ShortSword()
{
	minDamage_ = 8;
	maxDamage_ = 10;
	ItemText_ = "������ �ֵθ��� ���� ��";
	handedType_ = Weapon::HandedType::ONEHANDED;
	if (static_cast<int>(handedType_) == 1)
		printf("static cast �Ѽչ���\n");
	if (handedType_ == Weapon::HandedType::ONEHANDED)
			printf("�Ѽչ���\n");
	printf("����Ÿ��: %d\n", static_cast<int>(weaponType_));
	attackDelay_ = 1 / attackSpeed_;	// ���� ���, speed��2 �� attackdelay�� 0.5�� �Ǿ�. 0.5�ʸ��� 1���� �����ϴ°���.

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
	// �⺻ ������ 6��
	SetWeaponScale(6.0f * WSCALEY, 6.0f * WSCALEY);
}

ShortSword::~ShortSword()
{
}

void ShortSword::Update(Matrix V, Matrix P)
{
	// AnimationUpdate
	weapon_->SetPlay(0);

	// ���⼭ ���������� �ֵθ��� �ִϸ��̼��� �����ϰ� �� ��. (���콺 ��ġ�� ���.. ���)

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
	// ������ ���� 
	// ���� �ִϸ��̼��� ��ġ�� ���� 
}
