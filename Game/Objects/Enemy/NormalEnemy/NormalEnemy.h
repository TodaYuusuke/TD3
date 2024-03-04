#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class NormalEnemy: public IEnemy
{
public:
	void Init()override;
	void Update()override;

private:
	void Move(LWP::Math::Vector3 MoveVec)override;
	void Attack()override;

	lwp::Vector3 PlayerRot;
	AnimeWork attackWork = {
	.targetpoint = 1.0f,
	.speed = 0.05f,
	}; 
	AnimeWork attackStanbyWork = {
	.speed = 0.02f,
	};
	AnimeWork attackEndWork = {
	.targetpoint = 0.0f,
	.speed = 0.1f,
	};
};
