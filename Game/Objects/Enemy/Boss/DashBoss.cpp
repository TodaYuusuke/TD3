#include "DashBoss.h"
#include "Game/Objects/Player/Player.h"

void DashBoss::Init()
{
	// 当たり判定のインスタンス生成
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	// 色
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::RED);
	// 大きさ
	models_[0]->transform.scale = { 1,2,1 };
	// 当たり判定を有効化
	isActive_ = true;

	// 前のフレームでは攻撃可能か
	isPreAttack_ = false;

	// 攻撃のクールタイム
	attackWaitTime_ = kAttackWaitTime;
	// 現在の時間
	currentFrame_ = 0;
}

void DashBoss::Update()
{
	// 前のフレームの攻撃状態を更新
	isPreAttack_ = isAttack;

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
		break;
		// ダッシュ
	case Behavior::kDash:
		B_DashUpdate();
		break;
	}

	// 体を自機に向ける
	Aim();

	// 移動処理
	Move();
}

void DashBoss::SetPosition(lwp::Vector3 pos)
{
	models_[0]->transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void DashBoss::Move()
{
	lwp::Vector3 MoveVec = GetDirectVel();
	MoveVec.y = 0.0f;
	models_[0]->transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTime();
}

void DashBoss::Attack()
{

}

bool DashBoss::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0]->transform.translation - player_->GetWorldTransform()->translation).Length();
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
	models_[0]->transform.rotation.y = radian;
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

void DashBoss::B_DashInit() {
	isAttack = false;
}

void DashBoss::B_DashUpdate() {
	// 突進攻撃
	models_[0]->transform.translation += dashVel_ * kDashSpeedCoefficient;

	// 既定の時間を過ぎたら攻撃終了
	if (currentFrame_ >= kDashAttackAllFrame) {
		behaviorRequest_ = Behavior::kRoot;
	}

	currentFrame_++;
}

LWP::Math::Vector3 DashBoss::GetDirectVel() {
	return (player_->GetWorldTransform()->translation - models_[0]->transform.translation).Normalize();
}