#include "ImitationGame/framework.h"
#include "State/State.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "State/RunState.h"
#include "SkelDog.h"

SkelDog::SkelDog()
{
	state_ = new IdleState();
	
}

SkelDog::~SkelDog()
{
}

void SkelDog::Update(Matrix V, Matrix P)
{
	// IDLE, RUN, ATTACK(RUN 0 �� �������� �پ������)
	StateUpdate();


}

void SkelDog::Render()
{
}

void SkelDog::Reset()
{
}

void SkelDog::StateUpdate()
{
	state_->SwitchState(*this);
	state_->Action(*this);
}

void SkelDog::LeftMove()
{
}

void SkelDog::RightMove()
{
}

void SkelDog::Jump()
{
}

void SkelDog::Move()
{
	printf("�ذ� Move\n");
}

void SkelDog::Idle()
{
	printf("�ذ� Idle\n");
}

void SkelDog::Attack()
{
}
