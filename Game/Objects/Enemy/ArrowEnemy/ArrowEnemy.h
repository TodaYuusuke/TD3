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

	/// Setter

private:// プライベートなメンバ関数
	// 移動
	void Move(LWP::Math::Vector3 MoveVec)override;
	// 攻撃
	void Attack()override;
	// 対象を狙う
	void Aim();

public:// 定数


private:// メンバ変数
	// 矢
	std::list<Arrow*> arrows_;
};