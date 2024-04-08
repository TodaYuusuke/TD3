#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

/// <summary>
/// ジャンプ攻撃をする中ボス
/// </summary>
class JumpBoss : public IEnemy
{
private:// 構造体
	// 振るまい
	enum class Behavior {
		kRoot,		 // 通常状態
		kJump,		 // ジャンプ中
		kWaveAttack	 //	攻撃開始
	};

	// イージングで使用する
	struct EaseData {
		LWP::Math::Vector3 start;
		LWP::Math::Vector3 end;
	};

public:// パブリックなメンバ関数
	//*** 純粋仮想関数 ***//
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	///
	/// User Method
	/// 

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
	// 当たり判定の作成
	void CreateCollider() override;

	// 攻撃条件
	bool CheckAttackRange();

	// 対象を狙う
	void Aim();

#pragma region 振るまい
	// 通常状態の初期化
	void B_RootInit();
	// 通常状態の更新処理
	void B_RootUpdate();

	// ジャンプ状態の初期化
	void B_JumpInit();
	// ジャンプ状態の更新処理
	void B_JumpUpdate();

	// 衝撃波の初期化
	void B_WaveAttackInit();
	// 衝撃波の更新処理
	void B_WaveAttackUpdate();
#pragma endregion

	// float型のLerp
	float Lerp(const float& v1, const float& v2, float t);

private:// 定数
	// 攻撃する範囲
	const float kAttackRange = 15.0f;

	// 攻撃のクールタイム
	const float kAttackWaitTime = 120;
	// 目標座標に到達するまでの時間
	const float kJumpAllFrame = 30;
	// 攻撃の予備動作のフレーム
	const float kWaveAttackPreFrame = 120;
	// 攻撃の硬直
	const float kWaveAttackStunFrame = 120;
	// ジャンプ攻撃の全体フレーム
	const float kWaveAttackAllFrame = kWaveAttackPreFrame + kWaveAttackStunFrame;

	// ジャンプ攻撃の予備動作の上昇距離
	const float kJumpDirY = 3.0f;
	// ジャンプの最高座標
	const float kJumpHighestPoint = 3.0f;

private:// プライベートな変数
	// ジャンプ方向ベクトル
	LWP::Math::Vector3 jumpVel_;

	// ジャンプするときの目標座標
	LWP::Math::Vector3 jumpGoalPos_;
	// ジャンプフラグ
	bool isJump_;

	// ジャンプ挙動に使用するイージングデータ
	EaseData jumpEase_;

	// 現在のフレーム
	int currentFrame_;

	// 現在の振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};