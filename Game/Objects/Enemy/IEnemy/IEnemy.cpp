#include "IEnemy.h"

using namespace LWP::Object::Collider;

void IEnemy::Initialize()
{
	Init();

	CreateCollider();
	collider_.mask.SetBelongFrag(MaskLayer::Enemy);
}

void IEnemy::DyingAnimation()
{
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	float addx = distribution(randomEngine);
	float addz = distribution(randomEngine);
	if (deadFlame == 0) {
		deadAnime.targetpoint = models_[0].transform.translation;
		collider_.isActive = false;
	}
	if (deadFlame == 120) {
		isActive_ = false;
	}
	models_[0].transform.translation.x = deadAnime.targetpoint.x + addx * LWP::Info::GetDeltaTime();
	models_[0].transform.translation.z = deadAnime.targetpoint.z + addz * LWP::Info::GetDeltaTime();
	models_[0].transform.translation.y -= deadAnime.speed * LWP::Info::GetDeltaTime();
	deadFlame++;
}

void IEnemy::CreateCollider()
{
	// 当たり判定を設定
	collider_ = LWP::Object::Collider::AABB();
	// 当たり判定を取る
	collider_.CreateFromPrimitive(&models_[0]);
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
			IsDead_ = true;
		}
		});
}
