#include "HomingArrow.h"
#include "../../../Player/Player.h"

HomingArrow::HomingArrow(std::function<void(LWP::Math::Vector3)> func) {
	summonContrail_ = func;
}


HomingArrow::~HomingArrow() {

}

void HomingArrow::Init(lwp::WorldTransform transform) {
	// モデルの作成
	model_.LoadFile("cube/cube.obj");
	model_.commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::WHITE);
	model_.transform.translation = transform.translation;
	model_.transform.rotation = transform.rotation;
	model_.transform.scale = { 0.5f,0.5f,1.0f };
	model_.name = "HomingArrow!!";

	// 当たり判定を設定
	//aabb_ = new LWP::Object::Collider::AABB();
	aabb_.CreateFromPrimitive(&model_);
	aabb_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	aabb_.mask.SetHitFrag(MaskLayer::Player | MaskLayer::Layer3);
	aabb_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		if (!(data.state == OnCollisionState::None) &&
			data.hit &&
			((data.hit->mask.GetBelongFrag() & MaskLayer::Player) ||
				(data.hit->mask.GetBelongFrag() & MaskLayer::Layer3)))
		{
			Death();
		}
		});
	aabb_.isActive = true;

	// ホーミング開始時間(何も設定されていない場合はデフォルトの値を入れる)
	if (homingStartFrame_ == 0) {
		homingStartFrame_ = kHomingStartFrame;
	}
	// ホーミングする時間
	homingFrame_ = kHomingEndFrame;
	// ホーミング機能をoff
	isHoming_ = false;
	// ホーミング精度
	homingAccuracy_ = kLongDistHomingAccuracy;

	// 撃ちだす方向にモデルを回転
	model_.transform.rotation += shootingAngle_;

	// イージング開始時のベクトル
	LWP::Math::Vector3 velocity = { 0,0,1 };
	startEaseVel_ = (velocity * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.transform.rotation));
	startEaseVel_ = startEaseVel_.Normalize() * 30;
}

void HomingArrow::Update() {
	// 移動処理
	Attack();

	// 弾の消える条件
	if (deadTimer_ >= kLifeTime || model_.transform.translation.y <= 0)
	{
		Death();
	}

	if (static_cast<int>(deadTimer_) % 10 == 0) {
		//summonContrail_(model_.transform.GetWorldPosition());
	}

	// 寿命を進める
	deadTimer_++;
}

void HomingArrow::Attack() {
	// ホーミング開始時間の確認
	if (homingFrame_ <= kHomingEndFrame - homingStartFrame_) {
		isHoming_ = true;
	}
	// ホーミング終了時間の確認
	if (homingFrame_ <= 0) {
		isHoming_ = false;
	}

	// ホーミングの処理
	if (isHoming_) {
		// 距離によってホーミング精度を変える
		ChangeHomingAccuracy();
		// ホーミングの処理
		HomingUpdate();

		// 弾の向きを自機に向ける
		if (homingFrame_ >= kHomingEndFrame - homingStartFrame_ - kNormal2HomingFrame) {
			velocity_ = { 0,0,0 };
		}
	}
	// ホーミングしないときの処理
	else {
		// 方向ベクトル(モデルから見て正面に向かって移動)
		LWP::Math::Vector3 velocity = { 0,0,1 };
		// 回転行列の生成(発射角を含み計算)
		LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.transform.rotation);
		velocity = velocity * rotateMatrix;
		velocity_ = velocity.Normalize() * kSpeed;

		// 発射された弾はだんだん遅くなり止まる
		if (homingFrame_ >= kHomingEndFrame - homingStartFrame_) {
			LWP::Math::Vector3 endEaseVel = { 0,0,0 };
			velocity_ = LWP::Math::Vector3::Slerp(startEaseVel_, endEaseVel, deadTimer_ / homingStartFrame_);
		}
	}

	// 加算
	model_.transform.translation += velocity_ * LWP::Info::GetDeltaTimeF();

	// ホーミング開始時間を進める
	homingFrame_--;
	// 0よりも低くならないようにする
	homingFrame_ = min(max(homingFrame_, 0), kHomingEndFrame);

	attackWork.flag = true;
}

void HomingArrow::Death() {
	attackWork.flag = false;
}

void HomingArrow::HomingUpdate() {
	// 自機座標より少し上をホーミング
	LWP::Math::Vector3 playerPos = { player_->GetWorldTransform()->translation.x, player_->GetWorldTransform()->translation.y + 0.5f, player_->GetWorldTransform()->translation.z };
	// 弾から自機への方向ベクトルを算出
	LWP::Math::Vector3 toPlayer = playerPos - model_.transform.translation;
	// 正規化
	toPlayer = toPlayer.Normalize();
	velocity_ = velocity_.Normalize();

	// 球面線形保管により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = LWP::Math::Vector3::Slerp(velocity_, toPlayer, homingAccuracy_);

	velocity_ *= kSpeed;

#pragma region 弾の角度
	// Y軸周り角度(θy)
	model_.transform.rotation.y = std::atan2(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float velocityXZ;
	velocityXZ = sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	// X軸周りの角度(θx)
	model_.transform.rotation.x = std::atan2(-velocity_.y, velocityXZ);
#pragma endregion
}

void HomingArrow::ChangeHomingAccuracy() {
	// 自機との距離
	float distance = (model_.transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kHomingStrengthRange) {
		homingAccuracy_ = kShortDistHomingAccuracy;
	}
}