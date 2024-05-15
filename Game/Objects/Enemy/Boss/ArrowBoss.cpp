#include "ArrowBoss.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Object::Collider;

ArrowBoss::~ArrowBoss() {
	for (Arrow* arrow : normalArrows_) {
		delete arrow;
	}

	for (HomingArrow* arrow : homingArrows_) {
		delete arrow;
	}

	normalArrows_.clear();
	homingArrows_.clear();
}

void ArrowBoss::Init()
{
	// 当たり判定のインスタンス生成
	//models_.reserve(3);
	models_.emplace_back();
	models_[0].LoadFile("ArrowEnemy/ArrowEnemy.obj");
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
	//models_[0].commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::YELLOW);
	// 大きさ
	models_[0].transform.scale = {2,3,2};
	// 当たり判定を有効化
	isActive_ = true;

	shotCount_ = 0;

#pragma region ミサイル起動パーティクル
	// 形を決定
	missileContrail_.SetPrimitive<Primitive::Cube>();
	// 初期化処理
	missileContrail_.initFunction =
		[](Primitive::IPrimitive* primitive) {
		Object::ParticleData data;
		data.wtf.translation = primitive->transform.GetWorldPosition();
		data.wtf.rotation = primitive->transform.rotation;
		data.wtf.scale = primitive->transform.scale;

		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// 速度ベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		data.velocity = dir.Normalize() * 0.1f;

		// データを返す
		return data;
	};
	// 更新処理
	missileContrail_.updateFunction =
		[](Object::ParticleData* data) {
		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;
		data->wtf.translation.y += 0.001f * data->elapsedFrame;
		data->wtf.scale += {0.005f, 0.005f, 0.005f};
		
		// 速度は徐々に落とす
		data->velocity *= 0.9f;

		// 180フレーム経ったら削除
		return data->elapsedFrame > 240 ? true : false;
	};
	missileContrail_.isActive = true;
#pragma endregion

	hp_ = 50;
}

void ArrowBoss::Update()
{
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
			// 自機を狙う
		case Behavior::kAiming:
			B_AimingInit();
			break;
			// 撃つ
		case Behavior::kNormalShot:
			B_NormalShotInit();
			break;
			// ホーミング弾
		case Behavior::kHomingShot:
			B_HomingShotInit();
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
		// 自機を狙う
	case Behavior::kAiming:
		B_AimingUpdate();
		break;
		// 通常射撃
	case Behavior::kNormalShot:
		B_NormalShotUpdate();
		// ホーミング弾
	case Behavior::kHomingShot:
		B_HomingShotUpdate();
		break;
	}

	// 全ての弾の更新処理
	ArrowsUpdate();
}

void ArrowBoss::SetPosition(lwp::Vector3 pos)
{
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
	// 出現時にパーティクルを出す
	spawnEffect_(kNumSpawnParticle, models_[0].transform.translation);
}

LWP::Math::Vector3 ArrowBoss::GetDirectVel() {
	return (player_->GetWorldTransform()->translation - models_[0].transform.translation).Normalize();
}

void ArrowBoss::Move()
{
	lwp::Vector3 MoveVec = GetDirectVel();
	MoveVec.y = 0.0f;
	models_[0].transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTimeF();
}

void ArrowBoss::Attack()
{
	// 自機を狙う
	Aim();
#pragma region 通常弾
	if (behavior_ == Behavior::kNormalShot) {
		// 右に飛んでいく弾
		for (int i = 0; i < 2; i++) {
			// 弾を生成
			HomingArrow* homingArrow = new HomingArrow(
				[this](Math::Vector3 pos) {
					// 座標を変更してパーティクルを一つ生成
					*missileContrail_.Transform() = pos;
					missileContrail_.Add(1);
				}
			);
			// 上に弾を撃つ(x,y軸をランダムで少し回転)
			LWP::Math::Vector3 rotate = { -M_PI / 10.0f * (i + 1), 2.0f * M_PI / 3.0f, 0 };
			homingArrow->SetShootingAngle(rotate);
			// ホーミング開始時間
			homingArrow->SetHomingStartFrame(30);

			// 自機のアドレスを設定
			homingArrow->SetPlayer(player_);

			// 初期化
			homingArrow->Init(models_[0].transform);

			homingArrows_.push_back(homingArrow);
		}

		// 左に飛んでいく弾
		for (int i = 0; i < 2; i++) {
			// 弾を生成
			HomingArrow* homingArrow = new HomingArrow(
				[this](Math::Vector3 pos) {
					// 座標を変更してパーティクルを一つ生成
					*missileContrail_.Transform() = pos;
					missileContrail_.Add(1);
				}
			);
			// 上に弾を撃つ(x,y軸をランダムで少し回転)
			LWP::Math::Vector3 rotate = { -M_PI / 10.0f * (i + 1), -2.0f * M_PI / 3.0f, 0 };
			homingArrow->SetShootingAngle(rotate);
			// ホーミング開始時間
			homingArrow->SetHomingStartFrame(30);

			// 自機のアドレスを設定
			homingArrow->SetPlayer(player_);

			// 初期化
			homingArrow->Init(models_[0].transform);

			homingArrows_.push_back(homingArrow);
		}
	}
#pragma endregion

#pragma region ホーミング弾
	else if (behavior_ == Behavior::kHomingShot) {
		// 弾を生成
		HomingArrow* homingArrow = new HomingArrow(
			[this](Math::Vector3 pos) {
				// 座標を変更してパーティクルを一つ生成
				*missileContrail_.Transform() = pos;
				missileContrail_.Add(1);
			}
		);
		// 上に弾を撃つ(x,y軸をランダムで少し回転)
		LWP::Math::Vector3 rotate = RandomShootingAngle();
		homingArrow->SetShootingAngle(rotate);

		// 自機のアドレスを設定
		homingArrow->SetPlayer(player_);

		// 初期化
		homingArrow->Init(models_[0].transform);

		homingArrows_.push_back(homingArrow);
	}
#pragma endregion 
	isAttack = false;
}

void ArrowBoss::ArrowsUpdate() {
	// 通常弾の更新処理
	for (Arrow* arrow : normalArrows_) {
		arrow->Update();
	}
	normalArrows_.remove_if([](Arrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		return false;
		});

	// ホーミング弾の更新処理
	for (HomingArrow* arrow : homingArrows_) {
		arrow->Update();
	}
	homingArrows_.remove_if([](HomingArrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		return false;
		});
}

bool ArrowBoss::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

void ArrowBoss::Aim()
{
	// 自機に向かうベクトル
	LWP::Math::Vector3 targetVel = GetDirectVel();
	// 狙う対象に身体を向ける
	float radian = atan2(targetVel.x, targetVel.z);
	models_[0].transform.rotation.y = radian;
}

LWP::Math::Vector3 ArrowBoss::RandomShootingAngle() {
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	// x,y軸をランダムに回転
	std::uniform_real_distribution<float> distributionX(M_PI / 2.5f, 1.5f * M_PI / 2.5f);// 45~135度
	std::uniform_real_distribution<float> distributionY(0, 2 * M_PI);				   // 0~360度
	LWP::Math::Vector3 rotate = { -distributionX(randomEngine), distributionY(randomEngine), 0 };

	return rotate;
}

/// Behavior関数ここから↓
void ArrowBoss::B_RootInit() {
	// 攻撃状態解除
	isAttack = false;
	// 自機狙い状態のクールタイム
	stunFrame_ = kStunAllFrame;

	followCamera_->EndEffectFov();
}
void ArrowBoss::B_RootUpdate() {
	// 攻撃の待ち時間
	if (stunFrame_ <= 0) {
		isAiming_ = true;
	}

	// 攻撃範囲内にいるか
	if (CheckAttackRange()) {
		// 攻撃可能状態か
		if (isAiming_) {
			behaviorRequest_ = Behavior::kAiming;
		}
	}
	else {
		// 移動処理
		Move();
	}

	stunFrame_--;
}

void ArrowBoss::B_AimingInit() {
	// 自機狙い状態開始
	isAiming_ = true;
	attackWaitTime_ = kAimAllFrame;
}
void ArrowBoss::B_AimingUpdate() {
	// 体を自機に向ける
	Aim();

	// 既定の時間を過ぎたら行動開始
	if (attackWaitTime_ <= 0) {
		int randomBehavior = GenerateRandamNum(2, 3);
		behaviorRequest_ = static_cast<Behavior>(randomBehavior);//Behavior::kNormalShot;
	}

	attackWaitTime_--;
}

void ArrowBoss::B_NormalShotInit() {
	// 自機狙い状態解除
	isAiming_ = false;
	// 攻撃開始
	isAttack = true;
	// 射撃の全体フレーム
	shotFrame_ = kNormalShotAllFrame;
	// 射撃のディレイ
	shotDelay_ = kNormalShotDelayFrame;
	// 射撃回数
	shotCount_ = kMaxNormalShotCount;
}
void ArrowBoss::B_NormalShotUpdate() {
	// 射撃間隔
	if (shotDelay_ <= 0 && shotCount_ >= 1) {
		Attack();
		shotDelay_ = kNormalShotDelayFrame;
		shotCount_--;
	}
	// 既定の時間を過ぎたら攻撃終了
	if (shotFrame_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
	}

	shotDelay_--;
	shotFrame_--;
}

void ArrowBoss::B_HomingShotInit() {
	// 自機狙い状態解除
	isAiming_ = false;
	// 攻撃開始
	isAttack = true;
	// 射撃の全体フレーム
	shotFrame_ = kHomingShotAllFrame;
	// 射撃のディレイ
	shotDelay_ = kNormalShotDelayFrame;
	// 射撃回数
	shotCount_ = kMaxHomingShotCount;

	// 視野角を高くする
	followCamera_->StartEffectFov(kEffectFov);
}
void ArrowBoss::B_HomingShotUpdate() {
	// 射撃間隔
	if (shotDelay_ <= 0 && shotCount_ >= 1) {
		Attack();
		shotDelay_ = kHomingShotDelayFrame;
		shotCount_ --;
	}
	// 既定の時間を過ぎたら攻撃終了
	if (shotFrame_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
		//followCamera_->EndEffectFov();
	}

	shotDelay_--;
	shotFrame_--;
}