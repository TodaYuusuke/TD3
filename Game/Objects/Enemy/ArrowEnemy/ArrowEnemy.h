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
	void CreateCollider()override;
	///
	/// User Method
	/// 

	/// Getter

	/// Setter

private:// プライベートなメンバ関数

	// 移動
	void Move()override;
	// 攻撃
	void Attack()override;
	// 対象を狙う
	void Aim();
	// 攻撃範囲かどうか判定
	bool CheckAttackRange();
	// 最短角度補間
	float LerpShortAngle(float a, float b, float t);

public:// 定数
	// 攻撃する範囲
	const float kAttackRange = 20.0f;
	// 攻撃のクールタイム
	const int kAttackWaitTime = 240;

private:// メンバ変数
	// 矢
	std::list<Arrow*> arrows_;
};