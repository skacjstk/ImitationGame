#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "AttackState.h"

void AttackState::SwitchState(SkelDog& actor)
{
	// y값이 처음보다 낮아지면 공격이 끝난 것.
	// 땅이거나, 다른 선에 충돌했다면 attack을 즉시 중단하고 IDLE 로
	//actor.IsGround() || actor.IsConflicted() || 
	if (time_ > 1.0f || firstAttackPos.y > actor.GetPosition().y) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
		actor.moveSpeed = fabsf(actor.moveSpeed);	// Enter 에서 값을 바꿨는데, 그걸 정상화 하는 코드.
		time_ = 0.0f;
	}
}

void AttackState::Action(SkelDog& actor)
{
	//등가속도 운동 공식 버그: 다른쪽 bakFall 궤도가 이상해짐.
	Vector2 tempPosition = actor.GetPosition();

	tempPosition.x = firstAttackPos.x + (actor.moveSpeed * time_);
	tempPosition.y = firstAttackPos.y + (actor.jumpSpeed * time_) + (-0.5f * G * 150.0f * time_ * time_);
	// NEXT: 이거 gravityUpdate 나 GroundCheck 할때, y값을 또 바꾸거나 없애야 한다.
	//아마 WCtoVC에서 inverseMatrix 얘기 했던거보면... 그거 때매 y값이 뒤집어진 걸 수 있다.
	//식에 V0를 잘못 이해함. 벡터에서의 처음속도일뿐 좌표를 의미하는 건 아니다.
	time_ += TIMEMANAGER->Delta();
	actor.SetPosition(tempPosition);
}

void AttackState::Enter(SkelDog& actor)
{
	firstAttackPos = actor.GetPosition();

	float dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	actor.moveSpeed *= sign;
}
