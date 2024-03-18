#include "Player.h"

// ゲームシーン
#include "Game/Scenes/TitleScene.h"

Player::~Player()
{
	delete pInput_;
}

void Player::Initialize()
{
	// モデル読み込み
	demoModel_ = LWP::Resource::LoadModel("cube/cube.obj");
	//demoModel_->transform.Parent(&world_);
	demoModel_->isActive = true;
	demoModel_->name = "Player";

	demoModel_->transform.translation.z = -4.0f;

	// 武器を作成
	weapon_.reset(new Weapon);
	weapon_->Initialize();
	weapon_->SetParent(&demoModel_->transform);
	weapon_->SetTPointer(&easeT_);

	// 入力ハンドルを初期化
	pInput_ = new PlayerInput();
	pInput_->AssignCommands();

	// 状態の情報を設定
	InitDatas();

	// 居合攻撃の UI
	slashPanel_.reset(new SlashPanel);
	slashPanel_->Initialize();

	// コライダー生成
	CreateCollisions();
}

void Player::Update()
{
#ifdef DEMO
	// デバッグ表示
	DebugWindow();
#endif

	// コマンド動作を確認
	UpdateInput();

	// 状態を変えるか判別
	CheckBehavior();

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		t = 0.0f;
		easeT_ = 0.0f;
		switch (behavior_)
		{
		case Player::Behavior::Root:
			InitRoot();
			break;
		case Player::Behavior::Move:
			InitMove();
			break;
		case Player::Behavior::Slash:
			InitSlash();
			break;
		case Player::Behavior::Moment:
			InitMoment();
			break;
		case Player::Behavior::Damage:
			InitDamage();
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}
	switch (behavior_)
	{
	case Player::Behavior::Root:
		UpdateRoot();
		break;
	case Player::Behavior::Move:
		UpdateMove();
		break;
	case Player::Behavior::Slash:
		UpdateSlash();
		break;
	case Player::Behavior::Moment:
		UpdateMoment();
		break;
	case Player::Behavior::Damage:
		UpdateDamage();
		break;
	default:
		break;
	}
	t += (float)lwp::GetDeltaTime();
	weapon_->Update();
	slashPanel_->Update();
}

void Player::EndJust()
{
	LWP::Info::SetDeltaTimeMultiply(1.0f);
	//isJustSlashing_ = false;
	flag_.isJustSlashing_ = false;
	// 無敵切れは次の居合時にもなる
	colliders_.player_->isActive = true;
	pCamera_->ResetFov();
}

#pragma region CommandFunction

void Player::MoveFront()
{
	// 向いている方向へ変換するので単純にしている
	destinate_.z = 1.0f;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	flag_.isInputMove_ = true;
}

void Player::MoveBack()
{
	destinate_.z = -1.0f;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	flag_.isInputMove_ = true;
}

void Player::MoveLeft()
{
	destinate_.x = -1.0f;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	flag_.isInputMove_ = true;
}

void Player::MoveRight()
{
	destinate_.x = 1.0f;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	flag_.isInputMove_ = true;
}

void Player::Slash()
{
	commands_.push_back(Behavior::Slash);
}
// CommandFunc
#pragma endregion

#pragma region BehaviorFunc

void Player::InitRoot()
{
	//rootData_.maxTime_ = rootData_.cBASETIME;
	rootData_.maxTime_ = config_.Time_.ROOTBASE_;
	// 居合回数のリセット
	slashData_.relationSlash_ = 0u;
	//slashData_.maxRelation_ = slashData_.cMAXRELATION_;
	slashData_.maxRelation_ = config_.Count_.SLASHRELATIONMAX_;
	weapon_->SetBehavior(Weapon::Behavior::Root);
	// UI に反映
	slashPanel_->Reset();
}

void Player::InitMove()
{
	//moveData_.maxTime_ = moveData_.cBASETIME;
	moveData_.maxTime_ = config_.Time_.MOVEBASE_;
}

void Player::InitSlash()
{
	// デルタタイム変更
	EndJust();
	lwp::Vector3 vector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
	vector.y = 0.0f;
	vector = vector.Normalize();
	slashData_.vector_ = vector;
	//slashData_.maxTime_ = slashData_.cBASETIME;
	slashData_.maxTime_ = config_.Time_.SLASHBASE_;
	weapon_->SetBehavior(Weapon::Behavior::Slash);
	// 居合回数加算
	slashData_.relationSlash_++;
	// UI に反映
	slashPanel_->Slash();
	// 当たり判定を消去
	//colliders_.player_->isActive = false;
	flag_.isInvincible_ = true;
	// 武器の当たり判定を出す
	// カプセルの設定
	lwp::Vector3 start = demoModel_->transform.translation;
	lwp::Vector3 end = demoModel_->transform.translation;
	colliders_.weapon_->Create(start, end);
	colliders_.weapon_->radius = config_.Length_.WEAPONCOLLISIONRADIUS_;
	colliders_.weapon_->isActive = true;
	// ジャスト判定を作る
	colliders_.justSlash_->Create(start, end);
	// サイズ
	colliders_.justSlash_->radius = config_.Length_.JUSTCOLLISIONRADIUS_;
	colliders_.justSlash_->end = demoModel_->transform.translation + slashData_.vector_ * (config_.Speed_.SLASH_ * config_.Par_.JUSTENABLE_);
	colliders_.justSlash_->isActive = true;
}

void Player::InitMoment()
{
	// デルタタイム変更
	EndJust();
	momentData_.relationSlash_ = slashData_.relationSlash_;
	// 回数分フレームを加算
	//momentData_.maxTime_ = momentData_.cBASETIME + (momentData_.relationSlash_ * config_.Time_.MOMENTINCREMENT_);
	momentData_.maxTime_ = config_.Time_.MOMENTBASE_ + (momentData_.relationSlash_ * config_.Time_.MOMENTINCREMENT_);
	weapon_->SetBehavior(Weapon::Behavior::Moment);
	// 武器の判定を消す
	colliders_.weapon_->isActive = false;
}

void Player::InitDamage()
{
	// デルタタイム変更
	EndJust();
	colliders_.player_->isActive = false;
	//damageData_.maxTime_ = damageData_.cBASETIME;
	damageData_.maxTime_ = config_.Time_.DAMAGEBASE_;
}

void Player::UpdateRoot()
{
	if (rootData_.maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / rootData_.maxTime_;
}

void Player::UpdateMove()
{
	if (moveData_.maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / moveData_.maxTime_;
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
	moveVector.y = 0.0f;

	// モデル回転
	demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	moveVector = moveVector.Normalize() * (config_.Speed_.MOVE_ / moveData_.maxTime_) * (float)lwp::GetDeltaTime();

	demoModel_->transform.translation += moveVector;
}

void Player::UpdateSlash()
{
	// 無敵時間
	// ジャスト成立中
	//　無敵時間中
	colliders_.player_->isActive = (!flag_.isJustSlashing_ && config_.Time_.JUSTTAKETIME_ + config_.Time_.JUSTINVINCIBLE_ < t);
	// 判定を取れるようにする
	colliders_.justSlash_->isActive = t < config_.Time_.JUSTTAKETIME_;

	// 武器の判定を伸ばす
	colliders_.weapon_->end = demoModel_->transform.translation + slashData_.vector_ * config_.Length_.WEAPONPLUSCORRECTION_;
	if (slashData_.maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Moment;
	}
	easeT_ = t / slashData_.maxTime_;

	// 一定方向を向く
	lwp::Vector3 moveVector = slashData_.vector_;

	// モデル回転
	demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	moveVector = moveVector * (config_.Speed_.SLASH_ / slashData_.maxTime_) * (float)lwp::GetDeltaTime();

	demoModel_->transform.translation += moveVector;
}

void Player::UpdateMoment()
{
	if (momentData_.maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / momentData_.maxTime_;

	// 移動入力がされている時
	if (flag_.isInputMove_)
	{
		lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
		moveVector.y = 0.0f;

		// モデル回転
		demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

		//moveVector = moveVector.Normalize() * (cSPEEDMOMENT_ / momentData_.maxTime_) * lwp::GetDeltaTime();
		moveVector = moveVector.Normalize() * (config_.Speed_.MOMENT_ / momentData_.maxTime_) * (float)lwp::GetDeltaTime();

		demoModel_->transform.translation += moveVector;
	}
}

void Player::UpdateDamage()
{
	if (damageData_.maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / damageData_.maxTime_;
}

#pragma region BehaviorData

void Player::InitDatas()
{
	// 外部からの設定を取得
	InitConfigs();

	// 状態を初期状態に設定
	behavior_ = Behavior::Root;

	// 状態を設定
	InitRootData();
	InitMoveData();
	InitSlashData();
	InitMomentData();
	InitDamageData();
}

void Player::InitConfigs()
{
	InitSpeeds();
	InitTimes();
	InitLengths();
}

void Player::InitSpeeds()
{
}

void Player::InitTimes()
{
}

void Player::InitLengths()
{
}

void Player::InitRootData()
{
	//rootData_.cBASETIME = 0.5f;
	rootData_.maxTime_ = 0.0f;
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

void Player::CreateCollisions()
{
	CreatePlayerCollision();
	CreateWeaponCollision();
	CreateJustCollision();
}

void Player::CreatePlayerCollision()
{
	// 当たり判定を設定
	colliders_.player_ = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	// 武器との当たり判定を取る
	colliders_.player_->CreateFromPrimitive(demoModel_);
	// マスク
	colliders_.player_->mask.SetBelongFrag(MaskLayer::Player);
	// 敵または敵の攻撃
	colliders_.player_->mask.SetHitFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	// チョットした後隙
	// 別個で用意した当たった時の関数
	colliders_.player_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionPlayer(data); });

	colliders_.player_->isActive = true;
#ifdef DEMO
	colliders_.player_->name = "Player";
#endif
}

void Player::CreateWeaponCollision()
{
	// 当たり判定を設定
	colliders_.weapon_ = LWP::Common::CreateInstance<lwp::Collider::Capsule>();
	// 武器との当たり判定を取る
	colliders_.weapon_->Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	colliders_.weapon_->radius = config_.Length_.WEAPONCOLLISIONRADIUS_;
	// マスク
	colliders_.weapon_->mask.SetBelongFrag(MaskLayer::Layer3);
	colliders_.weapon_->mask.SetHitFrag(MaskLayer::Enemy);
	// 別個で用意した当たった時の関数
	colliders_.weapon_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionWeapon(data); });
	colliders_.weapon_->isActive = false;
#ifdef DEMO
	colliders_.weapon_->name = "Weapon";
#endif
}

void Player::CreateJustCollision()
{
	// ジャスト居合
	colliders_.justSlash_ = LWP::Common::CreateInstance<lwp::Collider::Capsule>();
	colliders_.justSlash_->Create(demoModel_->transform.translation, demoModel_->transform.translation);
	// マスク
	colliders_.justSlash_->mask.SetBelongFrag(MaskLayer::Player);
	colliders_.justSlash_->mask.SetHitFrag(MaskLayer::Layer2);
	// ジャスト居合したことを通知
	// 別個で用意した当たった時の関数
	colliders_.justSlash_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionJust(data); });
	// フラグオフ
	colliders_.justSlash_->isActive = false;

#ifdef DEMO
	colliders_.justSlash_->name = "Just";
#endif
}

void Player::OnCollisionPlayer(lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Trigger)
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
		// ジャスト判定の一瞬のみを取得している
		flag_.isJustSlashing_ = true;
		// ここをゲームシーンに変える
		TItleScene* const scene = dynamic_cast<TItleScene*>(pScene_);
		assert(scene);
		scene->StartJustSlash();
		pCamera_->ReduceFov();
		// 居合回数獲得(一回のみ)
		//if (slashData_.maxRelation_ <= slashData_.cMAXRELATION_)
		if (slashData_.maxRelation_ <= config_.Count_.SLASHRELATIONMAX_)
		{
			slashData_.maxRelation_++;
			slashPanel_->Just();
		}
	}
}

void Player::UpdateInput()
{
	// コマンドを積み重ねたものを取得
	pCommands_ = pInput_->HandleInput();

	// クリア
	commands_.clear();
	flag_.isInputMove_ = false;

	// 方向を作成
	lwp::Vector3 direct = destinate_;
	destinate_ = { 0.0f,0.0f,0.0f };

	// コマンドを実行
	// 実際には情報を一度すべて受け取る
	for (std::list<IPlayerCommand*>::iterator itr = pCommands_->begin();
		itr != pCommands_->end(); ++itr)
	{
		(*itr)->Exec(*this);

	}

	// キーボード入力として区別させる
	destinate_ = destinate_.Normalize() * 0.75f;

	// コントローラーの入力を合わせる
	float x = LWP::Input::Controller::GetLStick().x;
	float y = LWP::Input::Controller::GetLStick().y;
	if ((destinate_.x < 0 ? -destinate_.x : destinate_.x)
		< (x < 0 ? -x : x))
	{
		destinate_.x = x;
		commands_.push_back(Behavior::Move);
		// 移動キーが入力されている時通る
		flag_.isInputMove_ = true;
	}
	if ((destinate_.z < 0 ? -destinate_.z : destinate_.z)
		< (y < 0 ? -y : y))
	{
		destinate_.z = y;
		commands_.push_back(Behavior::Move);
		// 移動キーが入力されている時通る
		flag_.isInputMove_ = true;
	}
	destinate_ = destinate_.Normalize();

	// 方向がゼロだった場合は元に戻す
	if (destinate_.x == 0 && destinate_.z == 0)
	{
		destinate_ = direct.Normalize();
	}
}

void Player::CheckBehavior()
{
	// コマンドを設定
	if (reqBehavior_)
	{
		command_ = &reqBehavior_.value();
	}
	else
	{
		command_ = nullptr;
	}

	// 積み重ねたコマンドから実際の行動を決定する
	for (std::list<Behavior>::iterator itr = commands_.begin();
		itr != commands_.end(); ++itr)
	{
		if (command_)
		{
			// 優先度が高い方にする
			if (static_cast<uint32_t>(*command_) <= static_cast<uint32_t>(*itr))
			{
				command_ = &*itr;
			}
		}
		else
		{
			command_ = &*itr;
		}
	}

	// コマンドによって行動変化
	if (command_)
	{
		switch (*command_)
		{
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
			if ((behavior_ != Behavior::Slash || flag_.isJustSlashing_) &&
				slashData_.relationSlash_ < slashData_.maxRelation_)
			{
				reqBehavior_ = Behavior::Slash;
			}
			break;
		}
	}

}

void Player::DebugWindow()
{
	ImGui::Begin("PlayerWindow");

	ImGui::Text("How To Controll");
	ImGui::Bullet();
	ImGui::Text("WASD or LStick : MOVE");
	ImGui::Bullet();
	ImGui::Text("SPACE or ABXY  : SLASH");

	ImGui::Separator();

	ImGui::Text("Behavior : ");
	ImGui::SameLine();
	switch (behavior_)
	{
	case Player::Behavior::Root:
		ImGui::Text("ROOT");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.ROOTBASE_);
		ImGui::Text("MaxFrame  : %.3f", rootData_.maxTime_);
		break;
	case Player::Behavior::Move:
		ImGui::Text("MOVE");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.MOVEBASE_);
		ImGui::Text("MaxFrame  : %.3f", moveData_.maxTime_);
		break;
	case Player::Behavior::Slash:
		ImGui::Text("SLASH");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.SLASHBASE_);
		ImGui::Text("MaxFrame  : %.3f", slashData_.maxTime_);
		break;
	case Player::Behavior::Moment:
		ImGui::Text("MOMENT");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.MOMENTBASE_);
		ImGui::Text("MaxFrame  : %.3f", momentData_.maxTime_);
		break;
	case Player::Behavior::Damage:
		ImGui::Text("DAMAGE");
		ImGui::Text("BaseFrame : %.3f", config_.Time_.DAMAGEBASE_);
		ImGui::Text("MaxFrame  : %.3f", damageData_.maxTime_);
		break;
	default:
		break;
	}

	ImGui::Text("t : %.3f", t);

	ImGui::Separator();

	ImGui::Text("SlashRelation / MaxRelation");
	ImGui::Text("%d / %d", slashData_.relationSlash_, slashData_.maxRelation_);
	ImGui::Text("INCREMENTMOMENT : %.3f", config_.Time_.MOMENTINCREMENT_);

	ImGui::Separator();

	DebugSpeeds();
	DebugTimes();
	DebugLengths();
	//DebugCounts();
	DebugParcentages();

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
			ImGui::DragFloat("JUSTTAKE", &config_.Time_.JUSTTAKETIME_, 0.001f,0.0f,config_.Time_.SLASHBASE_);
			ImGui::DragFloat("JUSTINVINCIGLE", &config_.Time_.JUSTINVINCIBLE_, 0.001f,0.0f,config_.Time_.JUSTTAKETIME_);

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
		ImGui::SliderFloat("JUSTENABLE", &config_.Par_.JUSTENABLE_, 0.0f, 1.0f);

		ImGui::TreePop();
		ImGui::Separator();
	}
}
