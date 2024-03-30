#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

using namespace LWP::Object::Collider;

class Arrow
{
public:
	~Arrow();
	void Init(lwp::WorldTransform transform);
	void Update();
	void Attack();
	bool GetIsAlive() { return attackWork.flag; }
	void SetIsCollision(bool isActive) { aabb_->isActive = isActive; }
public:

	void Death();

private:// 定数
	// 弾の寿命
	const int kLifeTime = 600;

private:
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
};