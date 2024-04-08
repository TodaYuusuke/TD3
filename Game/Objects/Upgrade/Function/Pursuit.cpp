#include "Pursuit.h"

void Pursuit::Init()
{
}

bool Pursuit::Execution()
{

	if (interval > 0) {
		interval--;
		return true;
	}
	if (enemys_.empty()) {
		interval = kInterval;
		return false;
	}

	if (interval == 0) {
		models_.reserve(enemys_.size());
		for (int It = 0; It < enemys_.size(); It++) {
			models_.emplace_back();
			models_[It].LoadFile("cube/cube.obj");
			models_[It].transform.translation = enemys_[It]->GetWorldPosition();
			models_[It].transform.translation.y += 1.5f;
		}		
		//CreateCollider();
		interval--;
		return true;
	}
	else {
		interval--;
	}
	// 0以下になったら当たり判定を作る
	if (interval < 30) {
		interval = kInterval;
		// 最後にクリアする
		enemys_.clear();
		//models_.clear();
		//aabb_.clear();
		return false;
	}
}

void Pursuit::CreateCollider()
{
	for (int It = 0;It < enemys_.size(); It++) {
		// 当たり判定を設定
		AABB aabb;
		aabb.CreateFromPrimitive(&models_[It]);
		aabb.mask.SetBelongFrag(MaskLayer::Layer7);
		aabb.mask.SetHitFrag(MaskLayer::Enemy);
		aabb.name = "Pur";
		aabb.SetOnCollisionLambda([this](lwp::Collider::HitData data) {data;});
		aabb_.push_back(aabb);
	}
}
