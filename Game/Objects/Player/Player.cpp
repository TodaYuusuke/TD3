#include "Player.h"

// ゲームシーン
#include "Game/Scenes/TitleScene.h"

#include "Status/Derived/Idol.h"
#include "Status/Derived/Move.h"
#include "Status/Derived/Slash.h"
#include "Status/Derived/Moment.h"
#include "Status/Derived/Damage.h"

using Behavior = IStatus::Behavior;


void Player::Initialize()
{
	// モデル読み込み
	demoModel_.LoadFile("cube/cube.obj");
	//demoModel_->transform.Parent(&world_);
	demoModel_.isActive = true;
	demoModel_.name = "Player";

	demoModel_.transform.translation.z = -4.0f;

	// 武器を作成
	weapon_.reset(new Weapon);
	weapon_->Initialize();
	weapon_->SetParent(&demoModel_.transform);
	weapon_->SetTPointer(&easeT_);

	// 状態の情報を設定
	InitDatas();

	// 居合攻撃の UI
	slashPanel_.reset(new SlashPanel);
	slashPanel_->Initialize();

	invincibleTime_ = 0.0f;
	maxInvincibleTime_ = 0.0f;

	// コライダー生成
	CreateCollisions();

	// 状態作成
	statuses_.clear();

	statuses_.push_back(new Idol);
	statuses_.push_back(new Move);
	statuses_.push_back(new Slash);
	statuses_.push_back(new Moment);
	statuses_.push_back(new Damage);

	for (size_t i = 0; i < statuses_.size(); i++)
	{
		statuses_[i]->Init(this);
	}

	// 設定の初期化
	config_.Initialize();
	// パラメータの初期化
	parameter_.Initialize(&config_);
	// パラメータを反映させる
	parameter_.ResetParameter();
	// 今の状態を設定
	currStatus_ = statuses_[static_cast<size_t>(behavior_)];
}

void Player::Update()
{
#ifdef DEMO
	// デバッグ表示
	DebugWindow();
#endif

	// 移動入力を受け付ける
	CheckInputMove();

	// 攻撃入力を受け付ける
	CheckInputSlash();

	// 状態を変えるか判別
	CheckBehavior();

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		t = 0.0f;
		easeT_ = 0.0f;
		currStatus_ = statuses_[static_cast<size_t>(behavior_)];
		currStatus_->Reset();
		reqBehavior_ = std::nullopt;
	}
	// 状態の更新
	//statuses_[static_cast<size_t>(behavior_)]->Update();
	currStatus_->Update();
	t += (float)lwp::GetDeltaTime();
	weapon_->Update();
	slashPanel_->Update();


	//*** ここから下はフラグによって管理されている ***//

	colliders_.player_.Create(demoModel_.transform.translation + lwp::Vector3(0.0f, 0.5f, 0.0f));

	// 無敵時間確認
	if (flag_.isInvincible_)
	{
		// 無敵フレームを満たすか判断
		invincibleTime_ += (float)lwp::GetDeltaTime();
		if (maxInvincibleTime_ <= invincibleTime_)
		{
			// 無敵を切る
			flag_.isInvincible_ = false;
		}
	}
	// 無敵なのかどうか判断
	colliders_.player_.isActive = !flag_.isInvincible_;
}

void Player::StartJust()
{
	// ジャスト判定の一瞬のみを取得している
	flag_.isJustSlashing_ = true;
	// 無敵時間を加算
	maxInvincibleTime_ += config_.Time_.JUSTINVINCIBLEADD_;
	// ここをゲームシーンに変える
	pScene_->StartJustSlash();
	// 居合回数獲得(一回のみ)
	//if (slashData_.maxRelation_ <= slashData_.cMAXRELATION_)
	// パラメータによって上限を増やしてもいい
	if (slashData_.maxRelation_ <= (uint32_t)parameter_.slashNum)
	{
		slashData_.maxRelation_++;
		slashPanel_->Just();
	}
}

void Player::EndJust()
{
	//isJustSlashing_ = false;
	flag_.isJustSlashing_ = false;
	// 無敵切れは次の居合時にもなる
	colliders_.player_.isActive = true;
	// 終了したことを通知
	pScene_->EndJustSlash();
}

void Player::ApplyUpgrade(const UpgradeParameter& para)
{
	parameter_.ApplyUpgrade(para);
}


void Player::RegistStatus(IStatus::Behavior request)
{
	commands_.push_back(request);
}

lwp::Vector3 Player::GetVectorTranspose(const lwp::Vector3& vec)
{
	lwp::Vector3 vector = vec * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
	vector.y = 0.0f;
	return vector.Normalize();
}

#pragma region BehaviorFunc

#pragma region BehaviorData

void Player::InitDatas()
{
	// 外部からの設定を取得
	//InitConfigs();

	// 状態を初期状態に設定
	behavior_ = Behavior::Root;

	// 状態を設定
	InitRootData();
	InitMoveData();
	InitSlashData();
	InitMomentData();
	InitDamageData();
}

void Player::InitRootData()
{
	//rootData_.cBASETIME = 0.5f;
	rootData_.maxTime_ = 0.0f;
	rootData_.velocity_ = { 0.0f,0.0f,0.0f };
}

void Player::InitMoveData()
{
	//moveData_.cBASETIME = 0.1f;
	moveData_.maxTime_ = 0.0f;
}

void Player::InitSlashData()
{
	//slashData_.cBASETIME = 0.1f;
	slashData_.vector_ = { 0.0f,0.0f,1.0f };
	slashData_.maxTime_ = 0.0f;
	slashData_.relationSlash_ = 0u;
	//slashData_.cMAXRELATION_ = 3u;
	slashData_.maxRelation_ = 0u;
}

void Player::InitMomentData()
{
	//momentData_.cBASETIME = 0.5f;
	momentData_.maxTime_ = 0.0f;
	momentData_.relationSlash_ = 0u;
}

void Player::InitDamageData()
{
	//damageData_.cBASETIME = 0.5f;
	damageData_.maxTime_ = 0.0f;
}
// BeheviorData
#pragma endregion

// BehaviorFunc
#pragma endregion

#pragma region CollisionFunc

void Player::CreateCollisions()
{
	CreateJustCollision();
	CreatePlayerCollision();
	CreateWeaponCollision();
}

void Player::CreatePlayerCollision()
{
	// 当たり判定を設定
	colliders_.player_ = LWP::Object::Collider::AABB();
	// 武器との当たり判定を取る
	colliders_.player_.Create(demoModel_.transform.translation);
	// マスク
	colliders_.player_.mask.SetBelongFrag(MaskLayer::Player);
	// 敵または敵の攻撃
	colliders_.player_.mask.SetHitFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	// チョットした後隙
	// 別個で用意した当たった時の関数
	colliders_.player_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionPlayer(data); });

	colliders_.player_.isActive = true;
	flag_.isInvincible_ = false;
#ifdef DEMO
	colliders_.player_.name = "Player";
#endif
}

void Player::CreateWeaponCollision()
{
	// 当たり判定を設定
	//colliders_.weapon_ = LWP::Object::Collider::Capsule();
	// 武器との当たり判定を取る
	colliders_.weapon_.Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	colliders_.weapon_.radius = config_.Length_.WEAPONCOLLISIONRADIUS_;
	// マスク
	colliders_.weapon_.mask.SetBelongFrag(MaskLayer::Layer3);
	colliders_.weapon_.mask.SetHitFrag(MaskLayer::Enemy);
	// 別個で用意した当たった時の関数
	colliders_.weapon_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionWeapon(data); });
	colliders_.weapon_.isActive = false;
#ifdef DEMO
	colliders_.weapon_.name = "Weapon";
#endif
}

void Player::CreateJustCollision()
{
	// ジャスト居合
	//colliders_.justSlash_ = new LWP::Object::Collider::Capsule();
	colliders_.justSlash_.Create(demoModel_.transform.translation, demoModel_.transform.translation);
	// マスク
	colliders_.justSlash_.mask.SetBelongFrag(MaskLayer::Player);
	colliders_.justSlash_.mask.SetHitFrag(MaskLayer::Layer2);
	// ジャスト居合したことを通知
	// 別個で用意した当たった時の関数
	colliders_.justSlash_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionJust(data); });
	// フラグオフ
	colliders_.justSlash_.isActive = false;

#ifdef DEMO
	colliders_.justSlash_.name = "Just";
#endif
}

#pragma region OnCollisionFunc

void Player::OnCollisionPlayer(lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Press &&
		!flag_.isInvincible_ &&
		(data.hit->mask.GetBelongFrag() & (MaskLayer::Enemy | MaskLayer::Layer2)))
	{
		reqBehavior_ = Behavior::Damage;
	}
}

void Player::OnCollisionWeapon(lwp::Collider::HitData& data)
{
	data;
}

void Player::OnCollisionJust(lwp::Collider::HitData& data)
{
	if (!(data.state == OnCollisionState::None) &&
		!flag_.isJustSlashing_ && data.hit &&
		(data.hit->mask.GetBelongFrag() & MaskLayer::Layer2))
	{
		StartJust();
	}
}

#pragma endregion


#pragma endregion

void Player::CheckInputMove()
{
	lwp::Vector2 direct{ 0.0f,0.0f };
	if (lwp::Keyboard::GetPress(DIK_W))
	{
		direct.y += 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_S))
	{
		direct.y -= 1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_A))
	{
		direct.x -= 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_D))
	{
		direct.x += 1.0f;
	}
	direct += LWP::Input::Controller::GetLStick();
	direct = direct.Normalize();

	//// キーボード入力として区別させる
	//player_->destinate_ = direct.Normalize() * 0.75f;

	//// コントローラーの入力を合わせる
	//float x = LWP::Input::Controller::GetLStick().x;
	//float y = LWP::Input::Controller::GetLStick().y;
	//if ((player_->destinate_.x < 0 ? -player_->destinate_.x : player_->destinate_.x)
	//	< (x < 0 ? -x : x))
	//{
	//	player_->destinate_.x = x;
	//}
	//if ((player_->destinate_.z < 0 ? -player_->destinate_.z : player_->destinate_.z)
	//	< (y < 0 ? -y : y))
	//{
	//	player_->destinate_.z = y;
	//}
	//player_->destinate_ = player_->destinate_.Normalize();

	//// 方向がゼロだった場合は元に戻す
	//if (player_->destinate_.x == 0 && player_->destinate_.z == 0)
	//{
	//	player_->destinate_ = direct.Normalize();
	//}

	// そもそも移動入力が無かったらフラグを立てない
	flag_.isInputMove_ = !(direct.x == 0 && direct.y == 0);

	// 移動入力があった時に方向を更新する
		// 方向がゼロにならない保護
	if (flag_.isInputMove_)
	{
		destinate_.x = direct.x;
		destinate_.z = direct.y;
	}
}

void Player::CheckInputSlash()
{
	if (lwp::Keyboard::GetTrigger(DIK_SPACE) ||
		lwp::Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		flag_.isInputSlash_ = true;
	}
	else
		flag_.isInputSlash_ = false;
}


void Player::CheckBehavior()
{
	std::optional<Behavior> command = std::nullopt;

	// コマンドを設定
	command = reqBehavior_;

	// 積み重ねたコマンドから実際の行動を決定する
	for (std::list<Behavior>::iterator itr = commands_.begin();
		itr != commands_.end(); ++itr)
	{
		if (command)
		{
			// 優先度が高い方にする
			if (static_cast<uint32_t>(command.value()) <= static_cast<uint32_t>(*itr))
			{
				command = *itr;
			}
		}
		else
		{
			command = *itr;
		}
	}

	// コマンドによって行動変化
	if (command)
	{
		switch (command.value())
		{
		case Behavior::Root:
			reqBehavior_ = Behavior::Root;
			break;
		case Behavior::Move:
			// 移動は待機状態からの派生とか
			if (behavior_ == Behavior::Root ||
				behavior_ == Behavior::Move)
			{
				reqBehavior_ = Behavior::Move;
			}
			break;
		case Behavior::Slash:
			// 居合に入る条件を記述
			// 最大回数に達していないか
			if (slashData_.relationSlash_ < slashData_.maxRelation_)
				// if ((behavior_ != Behavior::Slash || flag_.isJustSlashing_) &&
				//	slashData_.relationSlash_ < slashData_.maxRelation_)
			{
				reqBehavior_ = Behavior::Slash;
			}
			break;
		case Behavior::Moment:
			reqBehavior_ = Behavior::Moment;
			break;
		case Behavior::Damage:
			reqBehavior_ = Behavior::Damage;
			break;
		}
	}

	// 確認したので中身を消す
	commands_.clear();

}

#pragma region DebugFunc

#if DEMO

void Player::DebugWindow()
{
	ImGui::Begin("PlayerWindow");

	ImGui::Text("How To Controll");
	ImGui::Bullet();
	ImGui::Text("WASD or LStick : MOVE");
	ImGui::Bullet();
	ImGui::Text("SPACE or A  : SLASH");

	ImGui::Separator();
	ImGui::Text("%f", rootData_.velocity_.x);
	ImGui::Text("%f", rootData_.velocity_.z);

	ImGui::Text("Behavior : ");
	ImGui::SameLine();
	switch (behavior_)
	{
	case Behavior::Root:
		ImGui::Text("ROOT");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.ROOTBASE_);
		ImGui::Text("MaxFrame  : %.3f", rootData_.maxTime_);
		break;
	case Behavior::Move:
		ImGui::Text("MOVE");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.MOVEBASE_);
		ImGui::Text("MaxFrame  : %.3f", moveData_.maxTime_);
		break;
	case Behavior::Slash:
		ImGui::Text("SLASH");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.SLASHBASE_);
		ImGui::Text("MaxFrame  : %.3f", slashData_.maxTime_);
		break;
	case Behavior::Moment:
		ImGui::Text("MOMENT");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.MOMENTBASE_);
		ImGui::Text("MaxFrame  : %.3f", momentData_.maxTime_);
		break;
	case Behavior::Damage:
		ImGui::Text("DAMAGE");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.DAMAGEBASE_);
		ImGui::Text("MaxFrame  : %.3f", damageData_.maxTime_);
		break;
	default:
		break;
	}

	ImGui::Text("t : %.3f", t);

	ImGui::Separator();

	ImGui::Text("SlashRelation");
	ImGui::Bullet();	ImGui::Text("Num / Max(Base)");
	ImGui::Bullet();	ImGui::Text("%d / %d(%d)", slashData_.relationSlash_, slashData_.maxRelation_, parameter_.slashNum);
	ImGui::Text("INCREMENTMOMENT : %.3f", config_.Time_.MOMENTINCREMENT_);
	ImGui::Text("Invincible : "); ImGui::SameLine();
	ImGui::Text(flag_.isInvincible_ ? "TRUE" : "FALSE");

	ImGui::Separator();

	if (ImGui::Button("Reset"))
	{
		parameter_.ResetParameter();
	}
	DebugSpeeds();
	DebugTimes();
	DebugLengths();
	//DebugCounts();
	DebugParcentages();

	ImGui::Separator();

	static float multi = 1.0f;
	if (ImGui::DragFloat("DeltaMulti", &multi, 0.001f))
	{
		lwp::SetDeltaTimeMultiply(multi);
	}
	static bool exec = false;
	ImGui::Checkbox("AppleyDelta", &exec);
	if (exec)
	{
		lwp::SetDeltaTimeMultiply(multi);
	}

	ImGui::End();
}

void Player::DebugSpeeds()
{
	if (ImGui::TreeNode("Speed"))
	{
		ImGui::DragFloat("MOVE", &config_.Speed_.MOVE_, 0.001f);
		ImGui::DragFloat("SLASH", &config_.Speed_.SLASH_, 0.001f);
		ImGui::DragFloat("MOMENT", &config_.Speed_.MOMENT_, 0.001f);

		ImGui::TreePop();
		ImGui::Separator();
	}
}


void Player::DebugTimes()
{
	if (ImGui::TreeNode("Time"))
	{
		if (ImGui::TreeNode("Base"))
		{
			ImGui::DragFloat("ROOT", &config_.Time_.ROOTBASE_, 0.001f);
			ImGui::DragFloat("MOVE", &config_.Time_.MOVEBASE_, 0.001f);
			ImGui::DragFloat("SLASH", &config_.Time_.SLASHBASE_, 0.001f);
			ImGui::DragFloat("MOMENT", &config_.Time_.MOMENTBASE_, 0.001f);
			ImGui::DragFloat("DAMAGE", &config_.Time_.DAMAGEBASE_, 0.001f);

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("ROOT"))
		{
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("MOVE"))
		{
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("SLASH"))
		{
			ImGui::DragFloat("JUSTTAKE", &config_.Time_.JUSTTAKETIME_, 0.001f, 0.0f, config_.Time_.SLASHBASE_);
			ImGui::DragFloat("JUSTINVINCIGLECORRECTION", &config_.Time_.JUSTINVINCIBLECORRECTION_, 0.001f, 0.0f, config_.Time_.SLASHBASE_ - config_.Time_.JUSTTAKETIME_);
			ImGui::DragFloat("JUSTINVINCIGLEADD", &config_.Time_.JUSTINVINCIBLEADD_, 0.001f, 0.0f);

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("MOMENT"))
		{
			ImGui::DragFloat("INCREMENT", &config_.Time_.MOMENTINCREMENT_, 0.001f);

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("DAMAGE"))
		{
			ImGui::DragFloat("INVINCIBLE", &config_.Time_.DAMAGEINVINCIBLE_, 0.001f);

			ImGui::TreePop();
			ImGui::Separator();
		}

		ImGui::TreePop();
		ImGui::Separator();
	}
}

void Player::DebugLengths()
{
	if (ImGui::TreeNode("Length"))
	{
		ImGui::DragFloat("WEAPONRADIUS", &config_.Length_.WEAPONCOLLISIONRADIUS_, 0.01f);
		ImGui::DragFloat("WEAPONCORRECTION", &config_.Length_.WEAPONPLUSCORRECTION_, 0.01f);
		ImGui::DragFloat("JUSTRADIUS", &config_.Length_.JUSTCOLLISIONRADIUS_, 0.01f);


		ImGui::TreePop();
		ImGui::Separator();
	}
}
void Player::DebugCounts()
{
	if (ImGui::TreeNode("Count"))
	{
		int i = config_.Count_.SLASHRELATIONMAX_;
		// 入力されたら
		if (ImGui::DragInt("SLASHRELATION", &i, 1, 10))
		{
			config_.Count_.SLASHRELATIONMAX_ = i;
		}

		ImGui::TreePop();
		ImGui::Separator();
	}
}

void Player::DebugParcentages()
{
	if (ImGui::TreeNode("Parcentage"))
	{
		ImGui::SliderFloat("JUSTENABLE", &config_.Parcent_.JUSTENABLE_, 0.0f, 1.0f);

		ImGui::TreePop();
		ImGui::Separator();
	}
}

#endif

#pragma endregion
