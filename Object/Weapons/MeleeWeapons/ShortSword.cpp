﻿#include "ImitationGame/framework.h"
#include "./Object/Weapons/MeleeWeapons/MeleeWeapon.h"
#include "./Physics/Collider.h"
#include "ShortSword.h"

ShortSword::ShortSword()
{
	minDamage_ = 8;
	maxDamage_ = 10;
	attackPerSecond_ = 3.03f;	// 공식 공속
	ItemText_ = "가볍고 휘두르기 편한 검";
	handedType_ = Weapon::HandedType::ONEHANDED;
//	if (static_cast<int>(handedType_) == 1)	// enum class 강제 형변환
//		printf("static cast 한손무기\n");
//	if (handedType_ == Weapon::HandedType::ONEHANDED)
//			printf("한손무기\n");
//	printf("무기타입: %d\n", static_cast<int>(weaponType_));
	currentAttackDelay_ = attackDelay_ = 1 / attackPerSecond_;	// 예를 들어, speed가2 면 attackdelay는 0.5가 되어. 0.5초마다 1번씩 공격하는거임.
	printf("공속 딜레이%f\n", attackDelay_);
	// 무기 대표이미지와 직접 사용할 애니메이션 생성
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
	strImage = IMAGE_FOLDER;	strImage += L"FX/SwingFX00.png";
	strShader = SHADER_FOLDER;	strShader += L"TextureColor.hlsl";
	attackFX_ = new Animation(strImage, strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		for (int i = 0; i < 3; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"FX/SwingFX0" + to_wstring(i) + L".png";
			pClip->AddFrame(attackFX_->GetTexture(), strImage, 0, 0, 40, 28, 0.1f);
		}
		attackFX_->AddClip(pClip);
	}
	attackFX_->SetStop();
	attackFX_->GetTexture()->UpdateColorBuffer(Color(0, 0, 0, 1),1);

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
	currentAttackDelay_ -= TIMEMANAGER->Delta();

	// 여기서 실질적으로 휘두르는 애니메이션을 관리하게 할 것. (마우스 위치점 기반.. 등등)
	AnimationUpdate();
	weapon_->SetPosition(GetWeaponPosition());
	weapon_->SetRotation(GetWeaponRotation());
	weapon_->SetScale(GetWeaponScale());
	weapon_->SetPivot(GetWeaponPivot());
	weapon_->Update(V, P);
	attackFX_->Update(V, P);
}

void ShortSword::Render()
{
	weapon_->Render();
	if(attackFX_->IsPlay())
		attackFX_->Render();
}

void ShortSword::Reset()
{
}

void ShortSword::Fire()
{
	if (!IsCanFire())
		return;
	// 공격 발사 성공시 아래 재생
	currentAttackDelay_ = attackDelay_;

	// Event: 공격 성공 이벤트
	attackCycle_ = -attackCycle_;
	// 무기의 공격 	// 손 위치 보정 : NExt: 0708 학원에서 용사 손 추가할예정	
	Vector2 beforePosition = GetWeaponPosition();
	Vector3 afterRotation;

	float gap = 100.0f;
	float fAngle = Mouse->GetAngleRelativeToMouse(beforePosition.x, beforePosition.y);
	float dRad = (fAngle * PI) / 180.0f; 
	beforePosition.x +=	cosf(dRad) * gap * WSCALEX;
	beforePosition.y +=  sinf(dRad) * gap * WSCALEY;
	afterRotation = Vector3(0.0f, 0.0f, fAngle -90.0f);
	attackFX_->SetPosition(beforePosition);
	attackFX_->SetScale(GetWeaponScale() * 0.8f);
	attackFX_->SetRotation(afterRotation);
	attackFX_->SetPlay(0, true);
}

// 주의사항: Player의 위치를 업데이트하고, Player가 WeaponPosition을 자신의 위치로 바꾼 직후에 AniUp을 수행해야 함.
void ShortSword::AnimationUpdate()
{	
	Vector2 beforePosition = GetWeaponPosition();
	Vector3 beforeRotation = GetWeaponRotation();
	Vector3 afterRotation;

	// 공격에 따른 각도 조절하기
	float fAngle = Mouse->GetAngleRelativeToMouse(beforePosition.x, beforePosition.y);
	afterRotation = Vector3(0.0f, 0.0f, fAngle + 90.0f * attackCycle_);

	Vector3 pivot = Vector3(-50.0f * WSCALEX, +15.0f * WSCALEY, 0.0f);	// Next: 콜라이더도 Pivot 적용 할..까? (무기 자체에 콜라이더면 그럴예정)
	SetWeaponPivot(pivot);

	// 손 위치 보정 : NExt: 0707 학원에서 용사 손 추가할예정
	beforePosition.x += 50.0f;
	beforePosition.y -= 20.0f;
	SetWeaponRotation(afterRotation);
	SetWeaponPosition(beforePosition);

	

}
