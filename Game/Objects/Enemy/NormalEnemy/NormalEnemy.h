#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
enum Model{
	Body = 0,
	L_Arm = 1,
	R_Arm = 2,
	MaxValue = 3
};

class NormalEnemy: public IEnemy
{
public:
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	/// Getter

	// チュートリアルモードが起動してるかを取得
	bool GetIsTutorial() { return isTutorial_; }

	/// Setter
	void SetEnemyHP(int stage) override;

	// チュートリアルモードを起動するかを設定
	void SetIsTutorial(bool isActive) { isTutorial_ = isActive; }

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
	const float kAttackRange = 5.0f;

	// 攻撃のクールタイム
	const int kAttackWaitTime = 180;

	// 移動の定数
	const float kMove = 5.0f;

private:
	lwp::Vector3 Rot;
	lwp::Vector3 EndRot;
	MotionWork attackRotWork = {
	.targetpoint = 1.0f,
	.speed = 0.05f,
	};
	MotionWork attackMoveWork = {
	.targetpoint = 1.0f,
	.speed = 0.05f,
	};
	MotionWork attackMoveEndWork = {
		.speed = 0.1f,
	};
	MotionWork attackStanbyWork = {
	.speed = 0.02f,
	};
	MotionWork attackEndWork = {
	.targetpoint = 0.0f,
	.speed = 0.1f,
	};

	// チュートリアルモード(エネミーマネージャからSetterで設定する)
	bool isTutorial_;
};
