#include "IEnemy.h"

#include "Game/Objects/Player/Player.h"
#include "Game/Objects/GameMask.h"

using namespace LWP;
using namespace LWP::Object::Collider;

void IEnemy::Initialize()
{
	Init();

	CreateCollider();

	// 死ぬときのアニメーション
	deadMotion_.Add(&models_[BODY].transform.translation, LWP::Math::Vector3{ 0,5,0 }, 0, 0.7f, LWP::Utility::Easing::Type::OutQuint);
	// 光の柱
	lightPillarMotion_.Add(&lightPillar_.transform.scale, LWP::Math::Vector3{ 1.5f,1.5f,1.5f }, 0, 0.1f)
		.Add(&lightPillar_.transform.scale, LWP::Math::Vector3{ -1.5f,-1.5f,-1.5f }, 0.1f, 0.1f);

	// 出現時の光の柱
	lightPillar_.texture = LWP::Resource::LoadTexture("particle/lightPillar.png");
	lightPillar_.name = "LightPillar";
	lightPillar_.transform.scale = { 1,100,1 };

	// ライティングをする
	models_[0].material.enableLighting = true;
}

void IEnemy::KnockBackUpdate()
{
	// プレイヤーパラメーターがtrueなら

	// 自機と反対の方向に移動させる
	// ノックバック判定が出ているかを確認
	if (player_->GetIsEnemyKnockBack())
	{
		// 自機との距離
		float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
		// ノックバックの範囲にいるなら自機と反対の方向に移動させる
		if (distance <= kKnockBackStartRange)
		{
			isKnockBack_ = true;
			// ノックバックする方向ベクトルを算出
			knockBackDir_ = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Normalize() * 0.4f;
			// 上には飛ばないようにする
			knockBackDir_.y = 0.0f;
		}
	}

	// ノックバックの移動処理
	if (isKnockBack_)
	{
		// ノックバック中
		if (knockBackTime_ <= 10)
		{
			// 方向ベクトルを加算
			models_[0].transform.translation = models_[0].transform.translation + knockBackDir_;
		}
		else
		{
			isKnockBack_ = false;
			knockBackTime_ = 0.0f;
		}

		knockBackTime_++;
	}
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
		deadMotion_.Start();
	}
	if (deadFlame == 20)
	{
		// エフェクト出す
#ifdef DEMO
		deadEffect_(16, models_[0].transform.translation);
#else
		deadEffect_(kNumDeadParticle, models_[0].transform.translation);
#endif

		isActive_ = false;
	}

	deadAnime.speed += 0.01f;

	models_[0].transform.rotation.y += deadAnime.speed;

	models_[0].transform.translation.x = deadAnime.targetpoint.x + addx * LWP::Info::GetDeltaTime();
	models_[0].transform.translation.z = deadAnime.targetpoint.z + addz * LWP::Info::GetDeltaTime();

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
		if (player_->parameter_.GetParameter().pursuitFlag && player_->GetPursuitFlag() && player_->parameter_.Attack.slashPower_ + 10 <= hp_)
		{
			player_->GetPursuit()->AddEnemy(this);
		}
		// 敵の攻撃に当たれる場合か
		// とりあえずなんかに当たった
		//wasHitCollision_ = true;
		// 居合攻撃のほかにも特別な奴とかありそうだから関数で分ける


		// 当たったのがプレイヤーの居合攻撃なら
		if (CheckSlash(data.hit->mask.GetBelongFrag()))
		{
			damege->Play(soundVolume);
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
#ifdef DEMO
	damageEffect_(16, models_[0].transform.translation);
#else
	damageEffect_(kNumDamageParticle, models_[0].transform.translation);
#endif // DEMO


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

void IEnemy::InitStaticVariable()
{
#pragma region ダメージを受けた時のエフェクト
	static LWP::Object::Particle damageParticle_;
	damageParticle_.SetPrimitive<Primitive::Cube>();
	damageParticle_.P()->transform.scale = { 0.0001f,0.001f, 0.0001f };
	damageParticle_.P()->material.enableLighting = true;
	damageParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::RED);
	damageParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = lwp::Vector3{ 0,1,0 } + primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		newData.wtf.scale = { 0.15f,0.15f, 0.15f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-200, 200);
		int dir2 = Utility::GenerateRandamNum<int>(-200, 200);
		int dir3 = Utility::GenerateRandamNum<int>(-200, 200);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		float multiply = Utility::GenerateRandamNum<int>(10, 30) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
	};
	damageParticle_.updateFunction = [](Object::ParticleData* data) {
		if (Info::GetDeltaTime() == 0.0f)
		{
			return false;
		}

		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;    // 速度ベクトルを加算
		data->wtf.translation.y += -9.8f / 80.0f;    // 重力を加算

		// 速度ベクトルを弱める
		data->velocity *= 0.9f;

		// 地面についたら薄く広がる
		if (data->wtf.translation.y <= 0.1f)
		{
			// 座標固定
			data->wtf.translation.y = 0.1f;
			// 速度をなくす
			data->velocity = { 0,0,0 };
			// x,z方向に伸ばす
			data->wtf.scale.x += 0.001f;
			data->wtf.scale.z += 0.001f;
		}
		else
		{
			// 速度ベクトルを弱める
			data->velocity *= 0.9f;
		}

		return data->elapsedFrame > 50 ? true : false;
	};
	damageParticle_.isActive = true;
	damageEffect_ = [&](int i, lwp::Vector3 pos) {
		damageParticle_.P()->transform = pos;
		damageParticle_.Add(i);
	};
#pragma endregion

#pragma region 死ぬとき
	static LWP::Object::Particle deadParticle_;
	deadParticle_.SetPrimitive<Primitive::Cube>();
	deadParticle_.P()->material.enableLighting = true;
	deadParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::RED);
	deadParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = lwp::Vector3{ 0,1,0 } + primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		// 大きさをランダムにする
		int scale = Utility::GenerateRandamNum<int>(25, 50);
		newData.wtf.scale = { scale / 200.0f, scale / 200.0f, scale / 200.0f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		// 係数
		float multiply = Utility::GenerateRandamNum<int>(20, 50) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
	};
	deadParticle_.updateFunction = [](Object::ParticleData* data) {
		if (Info::GetDeltaTime() == 0.0f)
		{
			return false;
		}

		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;    // 速度ベクトルを加算
		data->wtf.rotation += data->velocity;    // ついでに回転させとく


		// 20フレーム以降から重力を加算
		if (data->elapsedFrame > 20)
		{
			data->velocity.y += -9.8f / 800.0f;
			// yが0以下になったとき跳ねる
			if (data->wtf.translation.y <= 0.1f)
			{
				data->velocity.y *= -0.5f;
			}
		}
		else
		{
			// 速度ベクトルを弱める
			data->velocity *= 0.9f;
		}

		// ちょっとしたら検証開始

		// 速度が極端に遅くなったら終了フェーズ
		if (data->elapsedFrame > 25 &&
			data->velocity.y <= 0.01f && -0.01f <= data->velocity.y &&
			data->wtf.translation.y <= 0.15f && data->wtf.translation.y >= -0.15f)
		{
			data->velocity = { 0.0f,0.0f,0.0f };
			data->wtf.scale *= 0.9f;
			// もし完全に小さくなったなら終了
			if (data->wtf.scale.x <= 0.001f) { return true; }
		}

		return false;
	};
	deadParticle_.isActive = true;
	deadEffect_ = [&](int i, lwp::Vector3 pos) {
		deadParticle_.P()->transform = pos;
		deadParticle_.Add(i);
	};
#pragma endregion

#pragma region 出現時のエフェクト
	static LWP::Object::Particle spawnParticle_;
	spawnParticle_.SetPrimitive<Primitive::Cube>();
	spawnParticle_.P()->transform.scale = { 0.01f,0.01f, 0.01f };
	spawnParticle_.P()->material.enableLighting = true;
	spawnParticle_.P()->commonColor = new Utility::Color(0xCD853FFF);

	spawnParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		// 大きさをランダムにする
		int scale = Utility::GenerateRandamNum<int>(25, 40);
		newData.wtf.scale = { scale / 130.0f, scale / 130.0f, scale / 130.0f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 150.0f, dir3 / 100.0f };
		// 係数
		float multiply = Utility::GenerateRandamNum<int>(20, 50) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
	};
	spawnParticle_.updateFunction = [](Object::ParticleData* data) {
		if (Info::GetDeltaTime() == 0.0f)
		{
			return false;
		}
		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;    // 速度ベクトルを加算
		data->wtf.rotation += data->velocity;    // ついでに回転させとく


		// 20フレーム以降から重力を加算
		if (data->elapsedFrame > 10)
		{
			data->velocity.y += -9.8f / 800.0f;
			// yが0以下になったとき跳ねる
			if (data->wtf.translation.y <= 0.1f)
			{
				data->velocity.y *= -0.5f;
			}
		}
		else
		{
			// 速度ベクトルを弱める
			data->velocity *= 0.9f;
		}

		// ちょっとしたら検証開始

		// 速度が極端に遅くなったら終了フェーズ
		if (data->elapsedFrame > 15 &&
			data->velocity.y <= 0.01f && -0.01f <= data->velocity.y &&
			data->wtf.translation.y <= 0.15f && data->wtf.translation.y >= -0.15f)
		{
			data->velocity = { 0.0f,0.0f,0.0f };
			data->wtf.scale *= 0.9f;
			// もし完全に小さくなったなら終了
			if (data->wtf.scale.x <= 0.001f) { return true; }
		}

		return false;
	};
	spawnParticle_.isActive = true;
	spawnEffect_ = [&](int i, lwp::Vector3 pos) {
		spawnParticle_.P()->transform = pos;
		spawnParticle_.Add(i);
	};
#pragma endregion

	// 攻撃前
	static LWP::Object::Particle accumulateParticle_;
	accumulateParticle_.SetPrimitive<Primitive::Billboard2D>();
	accumulateParticle_.P()->texture = LWP::Resource::LoadTexture("particle/particle.png");
	accumulateParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = lwp::Vector3{ 0,-0.5f,0 } + primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		newData.wtf.scale = { 0.5f,0.5f, 0.0f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-10, 10);
		int dir2 = Utility::GenerateRandamNum<int>(-1, 5);
		int dir3 = Utility::GenerateRandamNum<int>(-10, 10);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 200.0f, dir3 / 100.0f };
		float multiply = Utility::GenerateRandamNum<int>(10, 50) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
	};
	accumulateParticle_.updateFunction = [&](Object::ParticleData* data) {
		if (Info::GetDeltaTime() == 0.0f) {
			return false;
		}

		// 経過フレーム追加
		data->elapsedFrame++;

		// 速度ベクトルを弱める
		data->velocity.x *= 0.9f;
		data->velocity.z *= 0.9f;

		// 重力を加算
		data->velocity.y += 9.8f / 2000.0f;

		// 速度ベクトルを加算
		data->wtf.translation += data->velocity;
		data->wtf.rotation += data->velocity;

		return data->elapsedFrame > 120 ? true : false;
	};
	accumulateParticle_.isActive = true;
	accumulateEffect_ = [&](int i, lwp::Vector3 pos) {
		accumulateParticle_.P()->transform = pos;
		accumulateParticle_.Add(i);
	};
}

void IEnemy::CheckSpawnEffect()
{
	if (lightPillarMotion_.isEnd())
	{
		lightPillar_.isActive = false;
	}
}

std::function<void(int, lwp::Vector3)> IEnemy::damageEffect_ = nullptr;
std::function<void(int, lwp::Vector3)> IEnemy::deadEffect_ = nullptr;
std::function<void(int, lwp::Vector3)> IEnemy::spawnEffect_ = nullptr;
std::function<void(int, lwp::Vector3)> IEnemy::accumulateEffect_ = nullptr;