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
	// Trigger だと別の判定が先に当たった時に動かない
	if (data.state == OnCollisionState::Press && isActive_ &&
		(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
	{
		// 敵の攻撃に当たれる場合か
		// とりあえずなんかに当たった
		//wasHitCollision_ = true;
		// 居合攻撃のほかにも特別な奴とかありそうだから関数で分ける


		// 当たったのがプレイヤーの居合攻撃なら
		if (CheckSlash(data.hit->mask.GetBelongFrag()))
		{
			return;
		}
	}
}

bool IEnemy::CheckSlash(uint32_t hitBelong)
{
	// 当たったのがプレイヤーの居合攻撃なら
	if (hitBelong & MaskLayer::Layer3 &&
		// まさか無敵じゃないよね?
		!isInvincible_)
	{
		// 攻撃力を参照してダメージを受ける
		DecreaseHP(player_->parameter_.Attack.slashPower_);
		// 居合攻撃だったので居合攻撃終わるまで無敵
		// 居合攻撃に続けて当たらないように判定を消す
		// とりあえず居合攻撃の 75% の時間あればいいよね?
		BecomeInvincible(player_->config_.Time_.SLASHBASE_);
		// 無量空処は居合攻撃の 25% にしとく
		BecomeUtopia(player_->config_.Time_.SLASHBASE_ * 0.25f);
		return true;
	}
	return false;
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

void IEnemy::BecomeInvincible(float time)
{
	isInvincible_ = true;
	invincibleTime_ = time;
}

void IEnemy::BecomeUtopia(float time)
{
	// とりあえず無量空処させとく
	isUtopia_ = true;
	utopiaTime_ = 0.1f;
}

void IEnemy::CheckFlags()
{
	// 無敵かどうかの方が重要
	// 無敵状態かどうかを見てる
	isInvincible_ = !(invincibleTime_ <= 0.0f);
	// 無敵時間を減算
	// 無敵じゃない時は 0.0f を入れている
	invincibleTime_ = isInvincible_ ?
		invincibleTime_ - lwp::GetDeltaTimeF() :
		0.0f;

	// 無敵なら当たり判定も消す
	collider_.isActive = !isInvincible_;

	// ここで無量空処されてるか見る
	isUtopia_ = !(utopiaTime_ <= 0.0f);
	// 無敵時間と同じようなもん
	utopiaTime_ = isUtopia_ ?
		utopiaTime_ - lwp::GetDeltaTimeF() :
		0.0f;
}

void IEnemy::DebugPrint()
{
	ImGui::Text(models_[0].name.c_str());
	ImGui::Separator();
	if (ImGui::Button("death"))IsDead_ = true;
	ImGui::Separator();
	ImGui::InputInt("HP", &hp_);
	//ImGui::Checkbox("WasHit", &wasHitCollision_);
	ImGui::Checkbox("IsUtopia", &isUtopia_);
	ImGui::Checkbox("IsInvincible", &isInvincible_);
	ImGui::Separator();
	ImGui::Text("utopia     : %.4f", utopiaTime_);
	ImGui::Text("invincible :%.4f", invincibleTime_);
}
