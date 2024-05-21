#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

/// <summary>
/// 突進タイプのボスキャラ
/// </summary>
class DashBoss : public IEnemy
{
private:// 構造体
	// 振るまい
	enum class Behavior {
		kRoot,	 // 通常状態
		kPreDash,// ダッシュ攻撃前の硬直
		kDash	 // ダッシュ中
	};

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

#pragma region 振るまい

	// 通常状態の初期化
	void B_RootInit();
	// 通常状態の更新処理
	void B_RootUpdate();

	// ダッシュ攻撃前の硬直の初期化
	void B_PreDashInit();
	// ダッシュ攻撃前の硬直の更新処理
	void B_PreDashUpdate();

	// ダッシュ状態の初期化
	void B_DashInit();
	// ダッシュ状態の更新処理
	void B_DashUpdate();

#pragma endregion

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
	const float kAttackRange = 20.0f;

	// 突進速度の係数
	const float kDashSpeedCoefficient = 1.0f;

	// 攻撃のクールタイム
	const int kAttackWaitTime = 240;
	// 突進攻撃の全体フレーム
	const int kDashAttackAllFrame = 30;
	// 攻撃前の硬直の全体フレーム
	const int kPreDashAllFrame = 60;

private:// プライベートな変数
	// ダッシュの方向ベクトル
	LWP::Math::Vector3 dashVel_;

	// 前のフレームでは攻撃可能か
	bool isPreAttack_;

	// 現在のフレーム
	int currentFrame_;

	// 現在の振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 攻撃前アニメーション
	LWP::Resource::Motion preAttackMotion_;

	// 突進攻撃前のエフェクト
	static std::function<void(int, lwp::Vector3)> accumulateEffect_;

	// パーティクルを外側に飛ばす
	bool isOutBlowOff_;
	lwp::Vector3 dirVel_;
};