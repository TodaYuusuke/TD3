#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class DashBoss : public IEnemy
{
public:// パブリックなメンバ関数
	//*** 純粋仮想関数 ***//
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	///
	/// User Method
	/// 

	// 攻撃条件
	bool CheckAttackRange();

	// 対象を狙う
	void Aim();

	/// Getter
	// 自機との方向ベクトルを算出(返り値は正規化している)
	LWP::Math::Vector3 GetDirectVel();

	/// Setter

private:// プライベートなメンバ関数
	//*** 純粋仮想関数 ***//
	// 移動
	void Move()override;
	// 攻撃
	void Attack()override;

private:// 定数
	// 攻撃する範囲
	const float kAttackRange = 10.0f;

	// 攻撃のクールタイム
	const int kAttackWaitTime = 120;

private:// プライベートな変数
	// ダッシュの方向ベクトル
	LWP::Math::Vector3 dashVel_;
};