#include "JumpBoss.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP;
using namespace LWP::Object::Collider;
using namespace LWP::Resource;

void JumpBoss::Init()
{
	// 当たり判定のインスタンス生成
	models_.reserve(3);
	models_.emplace_back();
	models_[0].LoadFile("cube/cube.obj");
	models_.emplace_back();
	models_[1].LoadFile("L_arm/L_arm.obj");
	models_.emplace_back();
	models_[2].LoadFile("R_arm/R_arm.obj");

	// 手のモデルをペアレント
	models_[1].transform.Parent(&models_[0].transform);
	models_[2].transform.Parent(&models_[0].transform);
	// 手のモデルの位置を設定
	models_[1].transform.translation = { -1.5f, 0.25f, 0.5f };
	models_[2].transform.translation = { 1.5f, 0.25f, 0.5f };

	// 色
	models_[0].commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::RED);
	// 大きさ
	models_[0].transform.scale = { 1,3,1 };
	// 当たり判定を有効化
	isActive_ = true;

	// 衝撃波を無効化
	isWaveAttack_ = false;

	// 攻撃のクールタイム
	attackWaitTime_ = kAttackWaitTime;
	// 現在の時間
	currentFrame_ = 0;

	// 攻撃モーションを追加
	jumpMotion_
		.Add(&models_[0].transform.scale, LWP::Math::Vector3{ 0, -2.0f, 0 }, 0, 0.05f)
		.Add(&models_[0].transform.scale, LWP::Math::Vector3{ 0, 2.0f, 0 }, 0.05f, 0.1f);
}

void JumpBoss::Update()
{
	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			B_RootInit();
			break;
			// ジャンプ
		case Behavior::kJump:
			B_JumpInit();
			break;
			// ジャンプ攻撃
		case Behavior::kWaveAttack:
			B_WaveAttackInit();
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
		// 移動処理
		Move();
		break;
		// ジャンプ
	case Behavior::kJump:
		B_JumpUpdate();
		break;
		// ジャンプ攻撃
	case Behavior::kWaveAttack:
		B_WaveAttackUpdate();
		break;
	}

	// 衝撃波の当たり判定を作成
	waveAttackCollider_.Create(models_[0].transform.translation);
	// 衝撃波の広がる処理
	if (isWaveAttack_) {
		WaveAttackSpread();
	}
}

void JumpBoss::SetPosition(lwp::Vector3 pos)
{
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

LWP::Math::Vector3 JumpBoss::GetDirectVel() {
	return (player_->GetWorldTransform()->translation - models_[0].transform.translation).Normalize();
}

void JumpBoss::Move()
{
	lwp::Vector3 MoveVec = GetDirectVel();
	MoveVec.y = 0.0f;
	models_[0].transform.translation += MoveVec / 20/* * 2.0f * LWP::Info::GetDeltaTime()*/;
}

void JumpBoss::Attack()
{

}

void JumpBoss::CreateCollider()
{
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
			isActive_ = false;
		}
		});


	// マスク
	waveAttackCollider_.mask.SetBelongFrag(MaskLayer::Enemy);
	waveAttackCollider_.mask.SetHitFrag(MaskLayer::Layer3);
	// 別個で用意した当たった時の関数
	waveAttackCollider_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollision(data); });
	// 当たり判定有効化
	waveAttackCollider_.isActive = false;

#ifdef DEMO
	waveAttackCollider_.name = "WaveAttack";
#endif
}

void JumpBoss::OnCollision(lwp::Collider::HitData& data)
{
	data;
	/*if (data.state == OnCollisionState::Press &&
		(data.hit->mask.GetBelongFrag() & MaskLayer::Layer2))
	{
		
	}*/
}

void JumpBoss::WaveAttackSpread() {
	waveAttackCollider_.radius += 0.1f;

	// 一定時間を過ぎると消える
	if (waveAttackFrame_ >= 240) {
		isWaveAttack_ = false;
		waveAttackCollider_.isActive = false;
	}

	waveAttackFrame_++;
}

bool JumpBoss::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

void JumpBoss::Aim()
{
	// 自機に向かうベクトル
	LWP::Math::Vector3 targetVel = GetDirectVel();
	// 狙う対象に身体を向ける
	float radian = atan2(targetVel.x, targetVel.z);
	models_[0].transform.rotation.y = radian;
}

void JumpBoss::B_RootInit() {
	attackWaitTime_ = kAttackWaitTime;
	currentFrame_ = 0;
	isAttack = false;
}
void JumpBoss::B_RootUpdate() {
	// 攻撃の待ち時間
	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
	else {
		isJump_ = true;
	}

	// 攻撃範囲内にいるか
	if (CheckAttackRange()) {
		// 攻撃可能状態か
		if (isJump_) {
			behaviorRequest_ = Behavior::kJump;
		}
	}
}

void JumpBoss::B_JumpInit() {
	isJump_ = false;
	currentFrame_ = 0;
	
	// ジャンプ開始座標を取得
	jumpEase_.start = { models_[0].transform.translation };
	// ジャンプの目標座標を取得
	jumpEase_.end = { player_->GetWorldTransform()->translation };
	jumpEase_.end.y += kJumpHighestPoint;

	// 攻撃モーション開始
	jumpMotion_.Start();
}
void JumpBoss::B_JumpUpdate() {
	float t = Utility::Easing::OutQuint(currentFrame_ / kJumpAllFrame);

	// ジャンプ挙動
	models_[0].transform.translation = LWP::Math::Vector3::Lerp(jumpEase_.start, jumpEase_.end, t);

	// 既定の時間を過ぎたら攻撃開始
	if (currentFrame_ >= kJumpAllFrame) {
		behaviorRequest_ = Behavior::kWaveAttack;
	}

	currentFrame_++;
}

void JumpBoss::B_WaveAttackInit() {
	currentFrame_ = 0;
	waveAttackFrame_ = 0;
	isAttack = true;
}
void JumpBoss::B_WaveAttackUpdate() {
	// 攻撃の予備動作
	if (currentFrame_ <= kWaveAttackPreFrame) {
		float t = Utility::Easing::InQuint(currentFrame_ / kWaveAttackPreFrame);

		// だんだん上に行く
		models_[0].transform.translation.y = Lerp(jumpEase_.end.y, jumpEase_.end.y + kJumpDirY,t);
		// モデルを回転
		models_[0].transform.rotation.y += Lerp(0, 2, t);
	}
	else {
		// 衝撃波開始
		models_[0].transform.translation.y = 0;
		isWaveAttack_ = true;
		waveAttackCollider_.isActive = true;
	}

	// 既定の時間を過ぎたら攻撃終了
	if (currentFrame_ >= kWaveAttackAllFrame) {
		behaviorRequest_ = Behavior::kRoot;
	}

	currentFrame_++;
}

float JumpBoss::Lerp(const float& v1, const float& v2, float t) {
	float result = v1 + (v2 - v1) * t;
	return result;
}