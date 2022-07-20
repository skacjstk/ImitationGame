#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "AttackState.h"

void AttackState::SwitchState(SkelDog& actor)
{
	// y���� ó������ �������� ������ ���� ��.
	// ���̰ų�, �ٸ� ���� �浹�ߴٸ� attack�� ��� �ߴ��ϰ� IDLE ��
	//actor.IsGround() || actor.IsConflicted() || 
	if (time_ > 1.0f || firstAttackPos.y > actor.GetPosition().y) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
		actor.moveSpeed = fabsf(actor.moveSpeed);	// Enter ���� ���� �ٲ�µ�, �װ� ����ȭ �ϴ� �ڵ�.
		time_ = 0.0f;
	}
}

void AttackState::Action(SkelDog& actor)
{
	//��ӵ� � ���� ����: �ٸ��� bakFall �˵��� �̻�����.
	Vector2 tempPosition = actor.GetPosition();

	tempPosition.x = firstAttackPos.x + (actor.moveSpeed * time_);
	tempPosition.y = firstAttackPos.y + (actor.jumpSpeed * time_) + (-0.5f * G * 150.0f * time_ * time_);
	// NEXT: �̰� gravityUpdate �� GroundCheck �Ҷ�, y���� �� �ٲٰų� ���־� �Ѵ�.
	//�Ƹ� WCtoVC���� inverseMatrix ��� �ߴ��ź���... �װ� ���� y���� �������� �� �� �ִ�.
	//�Ŀ� V0�� �߸� ������. ���Ϳ����� ó���ӵ��ϻ� ��ǥ�� �ǹ��ϴ� �� �ƴϴ�.
	time_ += TIMEMANAGER->Delta();
	actor.SetPosition(tempPosition);
}

void AttackState::Enter(SkelDog& actor)
{
	firstAttackPos = actor.GetPosition();

	float dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	float sign = copysign(1, dX);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	actor.moveSpeed *= sign;
}
