#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class ArrowEnemy : public IEnemy
{
public:
	void Init()override;
	void Update()override;

private:
	void Move(LWP::Math::Vector3 MoveVec)override;
};