#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "../ArrowEnemy/Arrow/Arrow.h"
#include "../ArrowEnemy/Arrow/HomingArrow.h"
#include "../../FollowCamera/FollowCamera.h"

class ArrowBoss : public IEnemy
{
private:// 構造体
	// 振るまい
	enum class Behavior {
		kRoot,	 // 通常状態
		kAiming, // 自機を狙う
		kNormalShot,   // 発射
		kHomingShot  // ホーミング弾
	};

public:// パブリックなメンバ関数
	//*** 純粋仮想関数 ***//
	~ArrowBoss() override;
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	/// Getter
	// 自機との方向ベクトルを算出(返り値は正規化している)
	LWP::Math::Vector3 GetDirectVel();

private:// プライベートなメンバ関数
	//*** 純粋仮想関数 ***//
	// 移動
	void Move()override;
	// 攻撃
	void Attack()override;

	// 全ての弾の更新処理
	void ArrowsUpdate();

	// 攻撃条件
	bool CheckAttackRange();

	// 対象を狙う
	void Aim();

	// 弾の発射角をランダムにする
	LWP::Math::Vector3 RandomShootingAngle();

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
	void B_NormalShotInit();
	// 射撃状態の更新処理
	void B_NormalShotUpdate();

	// ホーミング射撃状態の初期化
	void B_HomingShotInit();
	// ホーミング射撃状態の更新処理
	void B_HomingShotUpdate();
#pragma endregion

private:// 定数
#pragma region 行動のフレーム情報
	// 何もしない状態の全体フレーム
	const float kStunAllFrame = 120;
	// 自機狙い状態の全体フレーム
	const float kAimAllFrame = 120;
	// 通常射撃状態の全体フレーム
	const float kNormalShotAllFrame = 180;
	// ホーミング射撃の全体フレーム
	const float kHomingShotAllFrame = 180;

	// 通常弾の連射速度(次の弾を撃つまでの時間)
	const float kNormalShotDelayFrame = 0;
	// ホーミング弾の連射速度(次の弾を撃つまでの時間)
	const float kHomingShotDelayFrame = 6;
#pragma endregion

	// 通常弾数
	const int kMaxNormalShotCount = 4;
	// ホーミング弾数
	const int kMaxHomingShotCount = 9;

	// 攻撃する範囲
	const float kAttackRange = 40.0f;

	// 大技を行うときの視野角
	const float kEffectFov = 120;

private:// プライベートな変数
	// 何もしない状態の経過フレーム
	float stunFrame_;
	// 通常射撃状態の経過フレーム
	float shotFrame_;

	// 射撃のディレイ
	float shotDelay_;
	// 射撃回数
	int shotCount_;

	// 自機を狙う状態になるか
	bool isAiming_;

	// 現在の振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 通常弾
	std::list<Arrow*> normalArrows_;
	// ホーミング弾
	std::list<HomingArrow*> homingArrows_;



	// パーティクル
	LWP::Object::Particle missileContrail_;
};