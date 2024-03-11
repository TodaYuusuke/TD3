#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class ShieldEnemy : public IEnemy
{
public:
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

private:
	void Move(LWP::Math::Vector3 MoveVec)override;
	void Attack()override;

	MotionWork attackWork = {
		.speed = 0.05f,
	};
	MotionWork attackStanbyWork = {
		.speed = 0.05f,
	};
	MotionWork attackEndWork = {
		.speed = 0.1f,
	};
};