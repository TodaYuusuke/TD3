#include "Arrow.h"

void Arrow::Init(lwp::WorldTransform transform)
{
	// モデルの作成
	model_ = LWP::Common::CreateInstance<LWP::Primitive::Cube>();
	model_->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::WHITE);
	model_->transform.translation = transform.translation;
	model_->transform.rotation = transform.rotation;
	model_->transform.scale = { 0.5f,0.5f,1.0f };
	lwp::Vector3 point = { 0.0f,0.0f,1.0f };
	attackWork.targetpoint = point * lwp::Matrix4x4::CreateRotateXYZMatrix(transform.rotation);
	attackWork.targetpoint = attackWork.targetpoint.Normalize();

	// 当たり判定を設定
	aabb_ = LWP::Common::CreateInstance<AABB>();
	aabb_->CreateFromPrimitive(model_);
	aabb_->mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	aabb_->mask.SetHitFrag(MaskLayer::Player);
	aabb_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		if (!(data.state == OnCollisionState::None) &&
			data.hit &&
			(data.hit->mask.GetBelongFrag() & MaskLayer::Player))
		{
			Death();
		}
		});
	aabb_->isActive = true;
}

void Arrow::Update()
{
	// 移動処理
	Attack();

	// 弾が生存時間を越えていたら死ぬ
	if (deadTimer_ >= kLifeTime)
	{
		Death();
	}
	// 寿命を進める
	deadTimer_++;
#ifdef DEMO
	ImGui::Text("isAlive : %d", attackWork.flag);
#endif
}

void Arrow::Attack()
{
	model_->transform.translation += attackWork.targetpoint * attackWork.speed * LWP::Info::GetDeltaTime();
	attackWork.flag = true;
}

void Arrow::Death()
{
	attackWork.flag = false;
	model_->isActive = false;
	aabb_->isActive = false;
}

