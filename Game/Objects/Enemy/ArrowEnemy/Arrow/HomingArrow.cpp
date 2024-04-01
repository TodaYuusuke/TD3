#include "HomingArrow.h"
#include "../../../Player/Player.h"

HomingArrow::~HomingArrow() {
	delete aabb_;
}

void HomingArrow::Init(lwp::WorldTransform transform)
{
	// モデルの作成
	model_ = LWP::Primitive::Mesh();
	model_.LoadFile("cube/cube.obj");
	model_.commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::WHITE);
	model_.transform.translation = transform.translation;
	model_.transform.rotation = transform.rotation;
	model_.transform.scale = { 0.5f,0.5f,1.0f };
	model_.name = "Arrow!!";

	// 当たり判定を設定
	aabb_ = new LWP::Object::Collider::AABB();
	aabb_->CreateFromPrimitive(&model_);
	aabb_->mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	aabb_->mask.SetHitFrag(MaskLayer::Player | MaskLayer::Layer3);
	aabb_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		if (!(data.state == OnCollisionState::None) &&
			data.hit &&
			((data.hit->mask.GetBelongFrag() & MaskLayer::Player) ||
				(data.hit->mask.GetBelongFrag() & MaskLayer::Layer3)))
		{
			Death();
		}
		});

	aabb_->isActive = true;

	// ホーミングが開始する時間
	homingStartFrame_ = kHomingStartFrame;
	// ホーミング機能をoff
	isHoming_ = false;

	velocity_ = { 0,0,10 };
}

void HomingArrow::Update()
{
	// 移動処理
	Attack();

	// 弾が生存時間を越えていたら死ぬ
	if (deadTimer_ >= kLifeTime || model_.transform.translation.y <= 0)
	{
		Death();
	}
	// 寿命を進める
	deadTimer_++;
}

void HomingArrow::Attack()
{
	// ホーミング開始時間の確認
	if (homingStartFrame_ <= 0) {
		isHoming_ = true;
	}

	if (isHoming_) {
		// ホーミング機能がonになっているときに行う処理
		HomingUpdate();
	}
	else {
		// 弾が向いている方向に動く処理
		LWP::Math::Vector3 velocity = { 0,0,10 };
		// 回転行列の生成(発射角を含み計算)
		LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(shootingAngle_ + model_.transform.rotation);
		velocity = velocity * rotateMatrix;
		velocity_ = velocity.Normalize() * kSpeed;
	}
	// 加算
	model_.transform.translation += velocity_ * LWP::Info::GetDeltaTime();

	// ホーミング開始時間を進める
	homingStartFrame_--;
	// 0よりも低くならないようにする
	homingStartFrame_ = min(max(homingStartFrame_, 0), kHomingStartFrame);

	attackWork.flag = true;
}

void HomingArrow::Death()
{
	attackWork.flag = false;
}

void HomingArrow::HomingUpdate() {
	// 弾から自機への方向ベクトルを算出
	LWP::Math::Vector3 toPlayer = player_->GetWorldTransform()->translation - model_.transform.translation;
	// 正規化
	toPlayer = toPlayer.Normalize();
	velocity_ = velocity_.Normalize();

	// 球面線形保管により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = LWP::Math::Vector3::Lerp(velocity_, toPlayer, 0.05f);

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