#include "IEnemy.h"

using namespace LWP::Object::Collider;

void IEnemy::Initialize()
{
	Init();

	CreateCollider();
	collider_->mask.SetBelongFrag(MaskLayer::Enemy);
}

void IEnemy::CreateCollider()
{
	// 当たり判定を設定
	collider_ = LWP::Common::CreateInstance<AABB>();
	// 当たり判定を取る
	collider_->CreateFromPrimitive(models_[0]);
	// 今のところは何もしていない
	collider_->SetOnCollisionLambda([this](HitData data) {
		data;
		if (data.state == OnCollisionState::Trigger && isActive_)
		{
			isActive_ = false;
			models_[0]->isActive = false;
			collider_->isActive = false;
		}
		});
}
