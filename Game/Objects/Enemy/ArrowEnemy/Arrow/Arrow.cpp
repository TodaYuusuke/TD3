#include "Arrow.h"

#include "Game/Objects/GameMask.h"

void Arrow::Init(lwp::TransformEuler transform)
{
	// モデルの作成
	model_.LoadFile("cube/cube.obj");
	model_.commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::WHITE);
	model_.transform.translation = transform.translation;
	model_.transform.rotation = transform.rotation;
	model_.transform.scale = { 0.5f,0.5f,1.0f };
	model_.name = "Arrow!!";
	model_.material.enableLighting = true;

	lwp::Vector3 point = { 0.0f,0.0f,1.0f };
	attackWork.targetpoint = point * lwp::Matrix4x4::CreateRotateXYZMatrix(transform.rotation);
	attackWork.targetpoint = attackWork.targetpoint.Normalize();

	// 当たり判定を設定
	//aabb_ = new LWP::Object::Collider::AABB();
	aabb_.CreateFromPrimitive(&model_);
	aabb_.mask.SetBelongFrag(GameMask::EnemyAttack());
	aabb_.mask.SetHitFrag(GameMask::Player() | GameMask::Weapon());
	aabb_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		if (!(data.state == OnCollisionState::None) &&
			data.hit &&
			(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
		{
			Death();
		}
		});

	aabb_.isActive = true;
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
}

void Arrow::Attack()
{
	// 弾が向いている方向に動く処理
	LWP::Math::Vector3 velocity = { 0,0,1 };
	LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.transform.rotation);
	velocity = velocity * rotateMatrix;
	velocity_ = velocity.Normalize();
	velocity_ *= kNormalSpeed;

	// 加算
	model_.transform.translation += velocity_ * LWP::Info::GetDeltaTime();

	attackWork.flag = true;
}

void Arrow::Death()
{
	attackWork.flag = false;
}