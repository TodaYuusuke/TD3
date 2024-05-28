#include "DashBoss.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP;

void DashBoss::Init()
{
	// 当たり判定のインスタンス生成
	models_.emplace_back();
	models_[0].LoadFile("NormalEnemy/NormalEnemy.obj");

	// 大きさ
	models_[0].transform.scale = { 2,3,2 };
	// 当たり判定を有効化
	isActive_ = true;

	// 前のフレームでは攻撃可能か
	isPreAttack_ = false;

	// 攻撃のクールタイム
	attackWaitTime_ = kAttackWaitTime;
	// 現在の時間
	currentFrame_ = 0;

	// 攻撃前のモーション
	preAttackMotion_.Add(&models_[0].transform.scale, lwp::Vector3{ -1,-1,-1 }, 0, 0.3f, LWP::Utility::Easing::Type::OutQuart)
		.Add(&models_[0].transform.scale, lwp::Vector3{ 1,1,1 }, 1.4f, 0.2f, LWP::Utility::Easing::Type::OutQuart);

	// HP を設定
	hp_ = 60;

#pragma region パーティクル
	//// 攻撃前
	//static LWP::Object::Particle accumulateParticle_;
	//accumulateParticle_.SetPrimitive<Primitive::Cube>();
	//accumulateParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::YELLOW);
	//accumulateParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
	//	Object::ParticleData newData{};
	//	newData.wtf.translation = lwp::Vector3{ 0,-0.5f,0 } + primitive->transform.GetWorldPosition();
	//	newData.wtf.rotation = primitive->transform.rotation;
	//	newData.wtf.scale = { 0.5f,0.5f, 0.5f };

	//	// 速度ベクトルを生成
	//	int dir1 = Utility::GenerateRandamNum<int>(-10, 10);
	//	int dir2 = Utility::GenerateRandamNum<int>(-1, 5);
	//	int dir3 = Utility::GenerateRandamNum<int>(-10, 10);
	//	// 発射のベクトル
	//	Math::Vector3 dir{ dir1 / 100.0f,dir2 / 200.0f, dir3 / 100.0f };
	//	float multiply = Utility::GenerateRandamNum<int>(10, 50) / 100.0f;
	//	newData.velocity = dir.Normalize() * multiply;

	//	// パーティクル追加
	//	return newData;
	//};
	//accumulateParticle_.updateFunction = [&](Object::ParticleData* data) {
	//	if (Info::GetDeltaTime() == 0.0f) {
	//		return false;
	//	}

	//	// 経過フレーム追加
	//	data->elapsedFrame++;

	//	//// 方向ベクトル
	//	//lwp::Vector3 dirVel{};
	//	//// 方向ベクトルを算出(ただしy成分は除外)
	//	//dirVel = (data->wtf.translation - models_[0].transform.translation).Normalize() * 0.1f;
	//	//dirVel.y = data->velocity.y;
	//	// 速度ベクトルを弱める
	//	data->velocity.x *= 0.9f;
	//	data->velocity.z *= 0.9f;
	//
	//	//// パーティクルを外側へ飛ばす
	//	//if (isOutBlowOff_) {
	//	//	data->velocity = dirVel;
	//	//	// だんだん上昇速度を上げる
	//	//	data->velocity.x *= 1.5f;
	//	//	data->velocity.z *= 1.5f;
	//	//}

	//	// 重力を加算
	//	data->velocity.y += 9.8f / 2000.0f;

	//	// 速度ベクトルを加算
	//	data->wtf.translation += data->velocity;
	//	data->wtf.rotation += data->velocity;

	//	return data->elapsedFrame > 120 ? true : false;
	//};
	//accumulateParticle_.isActive = true;
	//accumulateEffect_ = [&](int i, lwp::Vector3 pos) {
	//	accumulateParticle_.P()->transform = pos;
	//	accumulateParticle_.Add(i);
	//};
#pragma endregion
}

void DashBoss::Update()
{
	// 前のフレームの攻撃状態を更新
	isPreAttack_ = isAttack;

	// 死亡時アニメーション
	// 死んだかどうかはすぐに判別
	if (IsDead_)
	{
		Dying();
		DyingAnimation();
		return;
	}

	// 無敵とかを調べる
	CheckFlags();
	// ここで無量空処されてる時は処理しない
	// アニメーションとかあるなら処理する
	if (isUtopia_)
	{
		return;
	}


	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			B_RootInit();
			break;
			// ダッシュ攻撃前の硬直
		case Behavior::kPreDash:
			B_PreDashInit();
			break;
			// ダッシュ
		case Behavior::kDash:
			B_DashInit();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// 更新処理
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		B_RootUpdate();
		// 体を自機に向ける
		Aim();
		break;
		// ダッシュ攻撃前の硬直
	case Behavior::kPreDash:
		B_PreDashUpdate();
		// 体を自機に向ける
		Aim();
		break;
		// ダッシュ
	case Behavior::kDash:
		B_DashUpdate();
		break;
	}

	if (preAttackMotion_.isEnd()) {
		isOutBlowOff_ = true;
		models_[0].transform.scale = { 2,3,2 };
	}
	else {
		isOutBlowOff_ = false;
	}

	// 移動処理
	Move();

	// スケールが極端な数値にならないように上限下限を設ける
	models_[0].transform.scale.x = std::clamp<float>(models_[0].transform.scale.x, 1, 2);
	models_[0].transform.scale.y = std::clamp<float>(models_[0].transform.scale.y, 1, 3);
	models_[0].transform.scale.z = std::clamp<float>(models_[0].transform.scale.z, 1, 2);
}

void DashBoss::SetPosition(lwp::Vector3 pos)
{
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
	// 出現時にパーティクルを出す
	SetSpawnEffect(models_[0].transform.translation);
}

void DashBoss::Move()
{
	dirVel_ = GetDirectVel();
	dirVel_.y = 0.0f;
	models_[0].transform.translation += dirVel_ * 2.0f * LWP::Info::GetDeltaTimeF();
}

void DashBoss::Attack()
{

}

bool DashBoss::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

void DashBoss::Aim()
{
	// 自機に向かうベクトル
	LWP::Math::Vector3 targetVel = GetDirectVel();
	// 狙う対象に身体を向ける
	float radian = atan2(targetVel.x, targetVel.z);
	models_[0].transform.rotation.y = radian;
}

void DashBoss::B_RootInit() {
	attackWaitTime_ = kAttackWaitTime;
	models_[0].transform.scale = { 2,3,2 };
	currentFrame_ = 0;
}

void DashBoss::B_RootUpdate() {
	// 攻撃の待ち時間
	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
	else {
		isAttack = true;
	}

	// 攻撃範囲内にいるか
	if (CheckAttackRange()) {
		// 攻撃可能状態か
		if (isAttack) {
			behaviorRequest_ = Behavior::kPreDash;
		}
	}
}

void DashBoss::B_PreDashInit() {
	currentFrame_ = 0;
	preAttackMotion_.Start();
}

void DashBoss::B_PreDashUpdate() {
	// パーティクルを出す時間
	if (currentFrame_ <= 60 && isActive_) {
		if (currentFrame_ % 2 == 0) {
			accumulateEffect_(16, models_[0].transform.translation);
		}

	}
	else if (currentFrame_ >= 60) {
		isOutBlowOff_ = true;
	}

	if (currentFrame_ >= 110) {
		// 自機との方向ベクトルを取得(ただしy方向のベクトルは取得しない)
		dashVel_ = GetDirectVel();
		dashVel_.y = 0;
	}

	if (currentFrame_ >= 120) {
		behaviorRequest_ = Behavior::kDash;
	}
	currentFrame_++;
}

void DashBoss::B_DashInit() {
	currentFrame_ = 0;
	isAttack = false;
}

void DashBoss::B_DashUpdate() {
	// 突進攻撃
	models_[0].transform.translation += dashVel_ * kDashSpeedCoefficient;

	// 既定の時間を過ぎたら攻撃終了
	if (currentFrame_ >= kDashAttackAllFrame) {
		behaviorRequest_ = Behavior::kRoot;
	}

	currentFrame_++;
}

LWP::Math::Vector3 DashBoss::GetDirectVel() {
	return (player_->GetWorldTransform()->translation - models_[0].transform.translation).Normalize();
}

//std::function<void(int, lwp::Vector3)> DashBoss::accumulateEffect_ = nullptr;