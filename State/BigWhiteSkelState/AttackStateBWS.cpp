#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/IdleStateBWS.h"
#include "AttackStateBWS.h"

void AttackStateBWS::SwitchState(BigWhiteSkel& actor)
{
	// 공격을 완료했다면, Next: 
	if (time_ > 1.0f) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
		actor.moveSpeed = fabsf(actor.moveSpeed);	// Enter 에서 값을 바꿨는데, 그걸 정상화 하는 코드.
		time_ = 0.0f;
	}
}

void AttackStateBWS::Action(BigWhiteSkel& actor)
{
	//등가속도 운동 공식 버그: 다른쪽 bakFall 궤도가 이상해짐.
	actor.Attack(dX);
	time_ += TIMEMANAGER->Delta();
	/*	
	//아마 WCtoVC에서 inverseMatrix 얘기 했던거보면... 그거 때매 y값이 뒤집어진 걸 수 있다.
	//식에 V0를 잘못 이해함. 벡터에서의 처음속도일뿐 좌표를 의미하는 건 아니다.
	tempPosition.x = firstAttackPos.x + (actor.moveSpeed * time_);
	tempPosition.y = firstAttackPos.y + (actor.jumpSpeed * time_) + (-0.5f * G * 150.0f * time_ * time_);
	*/
}

void AttackStateBWS::Enter(BigWhiteSkel& actor)
{
	time_ = 0.0f;
	dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	firstAttackPos = actor.GetPosition();
}
