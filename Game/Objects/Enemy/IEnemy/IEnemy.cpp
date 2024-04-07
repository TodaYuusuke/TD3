#include "IEnemy.h"

#include "Game/Objects/Player/Player.h"

using namespace LWP::Object::Collider;

void IEnemy::Initialize()
{
	Init();

	CreateCollider();
	//collider_.mask.SetBelongFrag(MaskLayer::Enemy);
}

void IEnemy::Death()
{
	IsDead_ = true;
	collider_.isActive = false;
	// 経験値を生成
	manager_->Create(models_[0].transform.translation);
}

void IEnemy::Dying()
{
	// 死んだときにしてほしい処理を書こうと思った
}

void IEnemy::DyingAnimation()
{
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	float addx = distribution(randomEngine);
	float addz = distribution(randomEngine);
	if (deadFlame == 0)
	{
		deadAnime.targetpoint = models_[0].transform.translation;
		collider_.isActive = false;
	}
	if (deadFlame == 120)
	{
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
	// 当たった時の処理
	collider_.SetOnCollisionLambda([this](HitData data) {OnCollision(data);	});
}

void IEnemy::OnCollision(const HitData& data)
{
	// 当たり判定が有効なら
	if (data.state == OnCollisionState::Trigger && isActive_)
		if
			(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag())
		{
			// 当たったのがプレイヤーの居合攻撃なら
			if (data.hit->mask.GetBelongFrag() & MaskLayer::Layer3)
			{
				// 攻撃力を参照してダメージを受ける
				DecreaseHP(player_->parameter_.Attack.slashPower_);
				return;
			}
		}
}

void IEnemy::DecreaseHP(int damage)
{
	// HP を減らす
	hp_ -= damage;
	// 死に至る
	if (hp_ <= 0)
	{
		Death();
	}
}
