#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "Arrow/Arrow.h"

class ArrowEnemy : public IEnemy
{
public:// パブリックなメンバ関数
	/// 
	/// Default Method
	/// 

	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	///
	/// User Method
	/// 

	/// Getter

	/// Setter

private:// プライベートなメンバ関数

	// コライダーを生成
	void CreateCollider() override;

	// 移動
	void Move()override;
	// 攻撃
	void Attack()override;
	// 対象を狙う
	void Aim();
	bool CheckAttackRange();

public:// 定数
	// 攻撃する範囲
	const float kAttackRange = 20.0f;
	// 攻撃のクールタイム
	const int kAttackWaitTime = 120;

private:// メンバ変数
	// 矢
	std::list<Arrow*> arrows_;
};