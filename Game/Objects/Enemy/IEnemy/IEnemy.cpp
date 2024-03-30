#include "IEnemy.h"

using namespace LWP::Object::Collider;

void IEnemy::Initialize()
{
	Init();

	CreateCollider();
	collider_.mask.SetBelongFrag(MaskLayer::Enemy);
}

void IEnemy::CreateCollider()
{
	// 当たり判定を設定
	collider_ = LWP::Object::Collider::AABB();
	// 当たり判定を取る
	//collider_->CreateFromPrimitive(&models_[0]);
	// マスク処理
	collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	collider_.mask.SetHitFrag(MaskLayer::Layer3);
	// 今のところは何もしていない
	collider_.SetOnCollisionLambda([this](HitData data) {
		data;
		if (data.state == OnCollisionState::Press && isActive_ &&
			data.hit &&
			(data.hit->mask.GetBelongFrag() & MaskLayer::Layer3))
		{
			isActive_ = false;
			//models_[0]->isActive = false;
			collider_.isActive = false;
		}
		});
}
