#include "DashBoss.h"
#include "Game/Objects/Player/Player.h"

void DashBoss::Init()
{
	// 当たり判定のインスタンス生成
//	models_.reserve(3);
	models_.emplace_back();
	models_[0].LoadFile("NormalEnemy/NormalEnemy.obj");
	//models_.emplace_back();
	//models_[1].LoadFile("L_arm/L_arm.obj");
	//models_.emplace_back();
	//models_[2].LoadFile("R_arm/R_arm.obj");

	//// 手のモデルをペアレント
	//models_[1].transform.Parent(&models_[0].transform);
	//models_[2].transform.Parent(&models_[0].transform);
	//// 手のモデルの位置を設定
	//models_[1].transform.translation = { -1.0f, 0.25f, 0.5f };
	//models_[2].transform.translation = { 1.0f, 0.25f, 0.5f };

	// 色
	//models_[0].commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLUE);
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

	//// 攻撃モーションを追加
	//attackMotion_[BODY]
	//	.Add(&models_[BODY].transform.scale, , 0, 0.05f)
	//	.Add(&models_[BODY].transform.scale, , 0.05f, 0.1f);
	//attackMotion_[L_ARM]
	//	.Add(&models_[L_ARM].transform.scale, LWP::Math::Vector3{ -1.0f, -1.0f, -1.0f }, 0, 0.05f)
	//	.Add(&models_[L_ARM].transform.scale, LWP::Math::Vector3{ 1.0f, 1.0f, 1.0f }, 0.05f, 0.1f);
	//attackMotion_[R_ARM]
	//	.Add(&models_[R_ARM].transform.scale, LWP::Math::Vector3{ -1.0f, -1.0f, -1.0f }, 0, 0.05f)
	//	.Add(&models_[R_ARM].transform.scale, LWP::Math::Vector3{ 1.0f, 1.0f, 1.0f }, 0.05f, 0.1f);

	// HP を設定
	hp_ = 60;
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
		// ダッシュ
	case Behavior::kDash:
		B_DashUpdate();
		break;
	}

	// 移動処理
	Move();
}

void DashBoss::SetPosition(lwp::Vector3 pos)
{
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
	// 出現時にパーティクルを出す
	spawnEffect_(kNumSpawnParticle, models_[0].transform.translation);
}

void DashBoss::Move()
{
	lwp::Vector3 MoveVec = GetDirectVel();
	MoveVec.y = 0.0f;
	models_[0].transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTimeF();
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
		// 自機との方向ベクトルを取得(ただしy方向のベクトルは取得しない)
		dashVel_ = GetDirectVel();
		dashVel_.y = 0;

		// 攻撃可能状態か
		if (isAttack) {
			behaviorRequest_ = Behavior::kDash;
		}
	}
}

void DashBoss::B_PreDashInit()
{
}

void DashBoss::B_PreDashUpdate()
{
}

void DashBoss::B_DashInit() {
	isAttack = false;
	for (int i = 0; i < BODYPARTSCOUNT; i++) {
		attackMotion_[i].Start();
	}
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
