#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class Arrow
{
public:
	~Arrow() = default;
	void Init(lwp::WorldTransform transform);
	void Update();
	void Attack();
	bool GetIsAlive() { return attackWork.flag; }
public:

private:// 定数
	// 弾の寿命
	const int kLifeTime = 120;

private:
	// 矢のモデル
	LWP::Primitive::IPrimitive* model_;

	MotionWork attackWork{
		.speed = 0.1f,
		.flag = false,
	};

	// 弾の寿命
	int deadTimer_;
};