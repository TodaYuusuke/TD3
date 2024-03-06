#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "Arrow/Arrow.h"

class ArrowEnemy : public IEnemy
{
public:
	void Init()override;
	void Update()override;

private:
	void Move(LWP::Math::Vector3 MoveVec)override;
	void Attack()override;
	std::list<std::unique_ptr<Arrow>> arrows_;
};