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

	///
	/// User Method
	/// 

	/// Getter
	// 狙う対象を取得
	const LWP::Math::Vector3& GetAimTarget() { return *target_; }

	/// Setter
	// 狙う対象を設定
	void SetAimTarget(LWP::Math::Vector3* target) { target_ = target; }

private:// プライベートなメンバ関数
	// 移動
	void Move(LWP::Math::Vector3 MoveVec)override;
	// 攻撃
	void Attack()override;
	// 対象を狙う
	void Aim();

private:// メンバ変数
	std::list<Arrow*> arrows_;
	// 狙う対象
	LWP::Math::Vector3* target_;
};