#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

using namespace LWP::Object::Collider;

class Player;
class HomingArrow
{
public:
	// デフォルトコンストラクタを禁止
	HomingArrow() = delete;
	HomingArrow(std::function<void(LWP::Math::Vector3)>);

	~HomingArrow();
	void Init(lwp::WorldTransform transform);
	void Update();
	void Attack();

	// 死ぬときに呼び出す
	void Death();

	//*** Getter ***//
	// 生きているかの取得
	bool GetIsAlive() { return attackWork.flag; }

	//*** Setter ***//
	// 当たり判定の設定
	void SetIsCollision(bool isActive) { aabb_.isActive = isActive; }

	/// <summary>
	/// 発射角を設定
	/// </summary>
	/// <param name="angle">発射角</param>
	void SetShootingAngle(LWP::Math::Vector3 angle) { shootingAngle_ = angle; }

	// ホーミング開始時間を設定
	void SetHomingStartFrame(int homingStartFrame) { homingStartFrame_ = homingStartFrame; }

	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }

private:// プライベートな関数
	// ホーミングの更新処理
	void HomingUpdate();
	// 距離に応じてホーミング精度を変動
	void ChangeHomingAccuracy();

private:// 定数
	// 弾の寿命
	const float kLifeTime = 300;

	// 何も設定されないときに呼ばれるホーミング開始時間
	const float kHomingStartFrame = 60;
	// ホーミング終了時間
	const float kHomingEndFrame = 180;

	// ホーミングの精度が変化する距離
	const float kHomingStrengthRange = 2.0f;
	// 自機との距離が遠い時のホーミング精度
	const float kLongDistHomingAccuracy = 0.6f;
	// 自機との距離が近い時のホーミング精度
	const float kShortDistHomingAccuracy = 0.01f;

	// 弾の速度
	const float kSpeed = 30.0f;

private:
	Player* player_;

	// 矢のモデル
	LWP::Primitive::Mesh model_;

	// AABB判定
	AABB aabb_;

	MotionWork attackWork{
		.speed = 5.0f,
		.flag = false,
	};

	// 弾の寿命
	float deadTimer_;

	// ホーミング弾の経過時間
	float homingFrame_;
	// ホーミング開始時間
	float homingStartFrame_;

	// ホーミング機能
	bool isHoming_;
	// ホーミング精度
	float homingAccuracy_;

	// 発射角
	LWP::Math::Vector3 shootingAngle_;
	// 方向ベクトル
	LWP::Math::Vector3 velocity_;

	// イージング開始時のベクトル
	LWP::Math::Vector3 startEaseVel_;
	// イージング終了時のベクトル
	LWP::Math::Vector3 endEaseVel_;

	// ミサイル起動パーティクル生成関数
	std::function<void(LWP::Math::Vector3)> summonContrail_ = nullptr;
};