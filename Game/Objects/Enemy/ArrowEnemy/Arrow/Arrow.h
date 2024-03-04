#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class Arrow
{
public:
	
	void Init(lwp::WorldTransform transform);
	void Update();
	void Attack();

private:
	std::vector<LWP::Primitive::IPrimitive*>models_;

	AnimeWork attackWork{
		.speed = 0.1f,
	};
};