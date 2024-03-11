#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class NormalEnemy: public IEnemy
{
public:
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

private:
	void Move(LWP::Math::Vector3 MoveVec)override;
	void Attack()override;

	lwp::Vector3 PlayerRot;
	MotionWork attackWork = {
	.targetpoint = 1.0f,
	.speed = 0.05f,
	}; 
	MotionWork attackStanbyWork = {
	.speed = 0.02f,
	};
	MotionWork attackEndWork = {
	.targetpoint = 0.0f,
	.speed = 0.1f,
	};
};
