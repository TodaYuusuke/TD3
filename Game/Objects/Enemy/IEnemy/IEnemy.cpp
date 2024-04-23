#include "IEnemy.h"

#include "Game/Objects/Player/Player.h"
#include "Game/Objects/GameMask.h"

using namespace LWP;
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
	//collider_ = LWP::Object::Collider::AABB();
	// 当たり判定を取る
	collider_.CreateFromPrimitive(&models_[0]);
	// マスク処理
	collider_.mask.SetBelongFrag(GameMask::Enemy() | GameMask::EnemyAttack());
	// 追撃のマスクしらない
	collider_.mask.SetHitFrag(GameMask::Weapon() | MaskLayer::Layer7);
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
		// 追撃クラスに登録
		if (player_->parameter_.GetParameter().pursuitFlag && player_->GetPursuitFlag()) {
			player_->GetPursuit()->AddEnemy(this);
		}
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
	// エフェクト出す
	damageEffect_(16, models_[0].transform.translation);
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
	utopiaTime_ = time;
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

	if (isInvincible_) {

	}

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

void IEnemy::InitStaticVariable() {
	static LWP::Object::Particle damageParticle_;
	damageParticle_.SetPrimitive<Primitive::Cube>();
	damageParticle_.P()->transform.scale = { 0.01f,0.01f, 0.01f };
	damageParticle_.P()->material.enableLighting = true;
	damageParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::RED);
	damageParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = lwp::Vector3{0,1,0} + primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		newData.wtf.scale = { 0.25f,0.25f, 0.25f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		newData.velocity = dir.Normalize() * 0.3f;

		// パーティクル追加
		return newData;
	};
	damageParticle_.updateFunction = [](Object::ParticleData* data) {
		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;    // 速度ベクトルを加算
		data->wtf.rotation += data->velocity;    // ついでに回転させとく
		data->wtf.translation.y += -9.8f / 80.0f;    // 重力を加算

		// 速度ベクトルを弱める
		data->velocity *= 0.9f;

		return data->elapsedFrame > 180 ? true : false;
	};
	damageParticle_.isActive = true;
	damageEffect_ = [&](int i, lwp::Vector3 pos) { 
		damageParticle_.P()->transform = pos;
		damageParticle_.Add(i);
	};
}

std::function<void(int, lwp::Vector3)> IEnemy::damageEffect_ = nullptr;
