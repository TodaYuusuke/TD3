#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class NormalEnemy: public IEnemy
{
public:
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

private:
	void Move()override;
	void Attack()override;
	void AttackAnimation();
	// 攻撃条件
	bool CheckAttackRange();
	// 対象を狙う
	void Aim();

private:// 定数
	// 攻撃する範囲
	const float kAttackRange = 2.0f;

	// 攻撃のクールタイム
	const int kAttackWaitTime = 120;

	// 移動の定数
	const float kMove = 2.0f;

private:
	lwp::Vector3 Rot;
	lwp::Vector3 EndRot;
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
