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
	AnimationUpdate();
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

// 주의사항: Player의 위치를 업데이트하고, Player가 WeaponPosition을 자신의 위치로 바꾼 직후에 AniUp을 수행해야 함.
void ShortSword::AnimationUpdate()
{
	Vector2 beforePosition = GetWeaponPosition();
	Vector3 beforeRotation = GetWeaponRotation();
	Vector3 afterRotation;
	Vector2 mousePosition = Mouse->GetPosition();
	CAMERA->WCtoVC(mousePosition);

	// 공격에 따른 각도 조절하기
	float fAngle;
	float fdX = mousePosition.x - beforePosition.x;
	float fdY = mousePosition.y - beforePosition.y;

	float dRad = atan2f(fdY, fdX);
	fAngle = (dRad * 180.0f) / PI;
//	afterRotation = Vector3(0.0f, 0.0f, fAngle + 90.0f * attackCycle_);
	afterRotation = Vector3((attackCycle_ -1) * 90.0f, 0.0f, 90.0f);
	printf("%f\n", fAngle);
	SetWeaponRotation(afterRotation);
	
	// 이에 따른 위치값 교정하기
	beforePosition.y += 50.0f;

	SetWeaponPosition(beforePosition);
}
