#include "ImitationGame/framework.h"
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

	// 기본 배율은 6배
	SetWeaponScale(6.0f * WSCALEY, 6.0f * WSCALEY);

	// 숏소드의 핸드 피벗은 이렇다. ( 만약 윈도우 크기가 변경된다면..? )
	leftHandPivot_.x = -10.0f * WSCALEX;
	leftHandPivot_.y = 15.0f * WSCALEY;
	leftHandPivot_.z = 0.0f;

	attackCollider_ = new Collider();
}

ShortSword::~ShortSword()
{
	SAFE_DELETE(attackCollider_);
}

void ShortSword::Update(Matrix V, Matrix P)
{
	EffectUpdate(V, P);
	// AnimationUpdate
	weapon_->SetPlay(0);
	currentAttackDelay_ -= TIMEMANAGER->Delta();

	// 여기서 실질적으로 휘두르는 애니메이션을 관리하게 할 것. (마우스 위치점 기반.. 등등)
	AnimationUpdate();
	weapon_->SetPosition(GetWeaponPosition());
	weapon_->SetRotation(GetWeaponRotation());
	weapon_->SetScale(GetWeaponScale());
	weapon_->GetAnimationClip(0)->SetClipPivot(GetWeaponPivot());
	weapon_->Update(V, P);

	if (attackFX_->IsPlay()) {
		attackFX_->Update(V, P);
		attackCollider_->Update(V, P);
	}
}

void ShortSword::Render()
{
	EffectRender();
	weapon_->Render();
	if (attackFX_->IsPlay()) {
		attackFX_->Render();
		attackCollider_->Render();
	}
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

	// 콜라이더 활성화
	attackCollider_->SetScale(attackFX_->GetTextureRealSize());
	attackCollider_->SetPosition(attackFX_->GetPosition());
	attackCollider_->SetRotation(attackFX_->GetRotation());
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	attackCollider_->Update(V, P);
	CheckAttack();

	Audio->Play("swing2", 1.0f);
}

void ShortSword::CheckAttack()
{
	GameActor* actor = nullptr;
	// 참조 전달로 해봤음.
	Room* tempRoom = SCENEMANAGER->GetCurrentScene()->GetCurrentRoom();
	if (tempRoom == nullptr)	// Town 등 일부 Scene에선 Room 형태가 아닐 수 있음.
		return;
	for (auto iter = tempRoom->roomObjects.begin(); iter != tempRoom->roomObjects.end(); ++iter) {
		actor = (GameActor*)iter->second;
		// actorType이 enemy가 아니면 애초에 충돌검사를 하지 않음.
		if (actor->actorData_.type == GameActor::ActorType::Enemy && actor->IsActive() == true) {
			if(Collider::Obb(attackCollider_, actor->GetCollider())){// IntersectionOBB 개선판			
				actor->Attacked(DamageDice());
				// 이펙트 반영, 중복시 기존걸 지워버리며 풀 관리
				if (slashEffect_ != nullptr) {
					(*slashEffect_)->SetStop();
					slashEffect_ = nullptr;
				}
				slashEffect_ = objectPool->GetSlashEffect();
				// 액터의 위치에서 내 위치를 뺀 라디안
				float fdX = actor->GetPosition().x - this->GetWeaponPosition().x;
				float fdY = actor->GetPosition().y - this->GetWeaponPosition().y;
				float dRad = atan2f(fdY, fdX);
				float fAngle = (dRad * 180.0f) / PI;
				(*slashEffect_)->SetRotation(0.0f,0.0f, fAngle + 90.0f);

				(*slashEffect_)->SetPosition(actor->GetPosition());
				(*slashEffect_)->SetScale(actor->GetScale().x * WSCALEX * 0.7f, actor->GetScale().y * WSCALEY * 0.7f);

				(*slashEffect_)->SetPlay(0);
			}// end Obb
		}// end ifEnemy	
	}


}

// 주의사항: Player의 위치를 업데이트하고, Player가 WeaponPosition을 자신의 위치로 바꾼 직후에 AniUp을 수행해야 함.
void ShortSword::AnimationUpdate()
{	
	Vector2 beforePosition = GetWeaponPosition();
	Vector3 beforeRotation = GetWeaponRotation();
	Vector3 afterRotation;

	// 공격에 따른 각도 조절하기
	float fAngle = Mouse->GetAngleRelativeToMouse(beforePosition.x, beforePosition.y);

	float vec = std::abs(owner_->GetRotation().y / 180.0f);	// 절대값을 주어야 180 , -180에 대해 자유로워짐.

	vec *= 2.0f;
	vec -= 1.0f;	// 정방 -1 역방 1 방향 검증용
	afterRotation = Vector3(0.0f, 90.0f * (vec + 1), -vec * fAngle + 90.0f * attackCycle_);

	Vector3 pivot = Vector3(-50.0f * WSCALEX, +15.0f * WSCALEY, 0.0f);	// Next: 콜라이더도 Pivot 적용 할..까? (무기 자체에 콜라이더면 그럴예정)
	SetWeaponPivot(pivot);

	beforePosition.x += 50.0f * WSCALEX * -vec;	// 좌우 위치값 보정
	beforePosition.y -= 20.0f * WSCALEY;
	SetWeaponRotation(afterRotation);
	SetWeaponPosition(beforePosition);
	LeftHandPoint_ = beforePosition;
	leftHandRot_ = afterRotation;

	// pivot은 이 무기 고정이기에 생성자 부분으로 올림.
}
