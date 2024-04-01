#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

using namespace LWP::Object::Collider;

class Player;
class HomingArrow
{
public:
	~HomingArrow();
	void Init(lwp::WorldTransform transform);
	void Update();
	void Attack();

	// ホーミングの更新処理
	void HomingUpdate();


	//*** Getter ***//

	bool GetIsAlive() { return attackWork.flag; }

	//*** Setter ***//

	void SetIsCollision(bool isActive) { aabb_->isActive = isActive; }
		
	/// <summary>
	/// 発射角を設定
	/// </summary>
	/// <param name="angle">発射角</param>
	void SetShootingAngle(LWP::Math::Vector3 angle) { shootingAngle_ = angle; }
	// 速度を設定
	//void SetVelocity(LWP::Math::Vector3 velocity) { velocity_ = velocity; }

	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }

public:
	void Death();

private:// 定数
	// 弾の寿命
	const int kLifeTime = 300;

	// ホーミング開始時間
	const int kHomingStartFrame = 60;

	// 通常弾の速度
	const float kSpeed = 30.0f;

private:
	Player* player_;

	// 矢のモデル
	LWP::Primitive::Mesh model_;

	// AABB判定
	AABB* aabb_;

	MotionWork attackWork{
		.speed = 5.0f,
		.flag = false,
	};

	// 弾の寿命
	int deadTimer_;

	// ホーミングが開始する時間
	int homingStartFrame_;
	// ホーミング機能
	bool isHoming_;

	// 発射角
	LWP::Math::Vector3 shootingAngle_;
	// 方向ベクトル
	LWP::Math::Vector3 velocity_;
};