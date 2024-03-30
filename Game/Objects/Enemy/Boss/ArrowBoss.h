#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "../ArrowEnemy/Arrow/Arrow.h"

class ArrowBoss : public IEnemy
{
private:// 構造体
	// 振るまい
	enum class Behavior {
		kRoot,	 // 通常状態
		kAiming, // 自機を狙う
		kShot    // 発射
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

	// 自機を狙う状態の初期化
	void B_AimingInit();
	// 自機を狙う状態の更新処理
	void B_AimingUpdate();

	// 射撃状態の初期化
	void B_ShotInit();
	// 射撃状態の更新処理
	void B_ShotUpdate();
#pragma endregion

	/// Getter
	// 自機との方向ベクトルを算出(返り値は正規化している)
	LWP::Math::Vector3 GetDirectVel();

	/// Setter

private:// プライベートなメンバ関数
	//*** 純粋仮想関数 ***//
	// コライダーを生成
	void CreateCollider() override;
	// 移動
	void Move()override;
	// 攻撃
	void Attack()override;

private:// 定数
	// 攻撃する範囲
	const float kAttackRange = 20.0f;

	// 連射速度(次の弾を撃つまでの時間)
	const int kShotDelayFrame = 12;

	// 何もしない状態の全体フレーム
	const int kStunAllFrame = 120;
	// 自機狙い状態の全体フレーム
	const int kAimAllFrame = 120;
	// 射撃状態の全体フレーム
	const int kShotAllFrame = 180;

	// 射撃する回数
	const int kMaxShotCount = 3;

private:// プライベートな変数
	// 何もしない状態の経過フレーム
	int stunFrame_;
	// 射撃状態の経過フレーム
	int shotFrame_;
	// 射撃のディレイ
	int shotDelay_;
	// 射撃回数
	int shotCount_;

	// 自機を狙う状態になるか
	bool isAiming_;

	// 現在の振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 弾
	std::list<Arrow*> arrows_;
};