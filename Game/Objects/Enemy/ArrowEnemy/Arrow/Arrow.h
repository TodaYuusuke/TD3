#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

using namespace LWP::Object::Collider;

class Arrow
{
public:
	~Arrow() = default;
	void Init(lwp::TransformEuler transform);
	void Update();
	void Attack();

	// ホーミングの更新処理
	void HomingUpdate();

	//*** Getter ***//
	bool GetIsAlive() { return attackWork.flag; }

	//*** Setter ***//
	void SetIsCollision(bool isActive) { aabb_.isActive = isActive; }
	// 向きを設定
	void SetRotate(LWP::Math::Vector3 rotate) { model_.worldTF.rotation = rotate; }
	// 速度を設定
	void SetVelocity(LWP::Math::Vector3 velocity) { velocity_ = velocity; }

public:
	void Death();

private:// 定数
	// 弾の寿命
	const float kLifeTime = 600;

	// 通常弾の速度
	const float kNormalSpeed = 10.0f;

private:
	// 矢のモデル
	LWP::Resource::RigidModel model_;

	// AABB判定
	AABB aabb_;

	MotionWork attackWork{
		.speed = 5.0f,
		.flag = true,
	};

	// 弾の寿命
	float deadTimer_;

	// 方向ベクトル
	LWP::Math::Vector3 velocity_;
};