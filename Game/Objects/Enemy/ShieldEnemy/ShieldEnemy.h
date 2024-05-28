#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class ShieldEnemy : public IEnemy
{
public:
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	void SetEnemyHP(int stage) override;
private:
	void Move()override;
	void Attack()override;
	void AttackAnimation();
	// 攻撃条件
	bool CheckAttackRange();
	// 対象を狙う
	void Aim();

	// 攻撃する範囲
	const float kAttackRange = 2.0f;

	// 攻撃のクールタイム
	const int kAttackWaitTime = 120;

private:
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