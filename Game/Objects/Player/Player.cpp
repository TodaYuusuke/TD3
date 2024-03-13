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

	CreateCollision();
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
			//rootData_->time_ = 0.0f;
			rootData_->maxTime_ = rootData_->cBASETIME;
			// 居合回数のリセット
			slashData_->relationSlash_ = 0u;
			slashData_->maxRelation_ = slashData_->cMAXRELATION_;
			weapon_->SetBehavior(Weapon::Behavior::Root);
			// UI に反映
			slashPanel_->Reset();
			break;
		case Player::Behavior::Move:
			//moveData_->time_ = 0.0f;
			moveData_->maxTime_ = moveData_->cBASETIME;
			break;
		case Player::Behavior::Slash:
			// デルタタイム変更
			EndJust();
			//slashData_->time_ = 0.0f;
			lwp::Vector3 vector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
			vector.y = 0.0f;
			vector = vector.Normalize();
			slashData_->vector_ = vector;
			slashData_->maxTime_ = slashData_->cBASETIME;
			weapon_->SetBehavior(Weapon::Behavior::Slash);
			// 居合回数加算
			slashData_->relationSlash_++;
			// UI に反映
			slashPanel_->Slash();
			// 当たり判定を消去
			playerCollision_->isActive = false;
			// 武器の当たり判定を出す
			// カプセルの設定
			lwp::Vector3 start = demoModel_->transform.translation;
			lwp::Vector3 end = demoModel_->transform.translation;
			weaponCollision_->Create(start, end);
			weaponCollision_->radius = cRADIUSWEAPONCOLLISION_;
			weaponCollision_->isActive = true;
			// ジャスト判定を作る
			justCollision_->Create(demoModel_->transform.translation);
			// サイズ
			justCollision_->max = playerCollision_->max + lwp::Vector3(1.0f, 1.0f, 1.0f);
			justCollision_->min = playerCollision_->min - lwp::Vector3(1.0f, 1.0f, 1.0f);
			break;
		case Player::Behavior::Moment:
			//momentData_->time_ = 0.0f;
			momentData_->relationSlash_ = slashData_->relationSlash_;
			// 回数分フレームを加算
			momentData_->maxTime_ = momentData_->cBASETIME + (momentData_->relationSlash_ * cTIMEINCREMENTMOMENT_);
			weapon_->SetBehavior(Weapon::Behavior::Moment);
			// 武器の判定を消す
			weaponCollision_->isActive = false;
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
	default:
		break;
	}
	t += lwp::GetDeltaTime();
	weapon_->Update();
	slashPanel_->Update();
}

void Player::EndJust()
{
	LWP::Info::SetDeltaTimeMultiply(1.0f);
	isJustSlashing_ = false;
	// 無敵切れは次の居合時にもなる
	playerCollision_->isActive = true;
	pCamera_->ResetFov();
}

void Player::MoveFront()
{
	// 向いている方向へ変換するので単純にしている
	destinate_.z = 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	isInputMove_ = true;
}

void Player::MoveBack()
{
	destinate_.z = -1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	isInputMove_ = true;
}

void Player::MoveLeft()
{
	destinate_.x = -1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	isInputMove_ = true;
}

void Player::MoveRight()
{
	destinate_.x = 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
	// 移動キーが入力されている時通る
	isInputMove_ = true;
}

void Player::Slash()
{
	//destinate_.z += 1.0f;
	//reqBehavior_ = Behavior::Slash;
	commands_.push_back(Behavior::Slash);
}

void Player::UpdateRoot()
{
	if (rootData_->maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / rootData_->maxTime_;
	//rootData_->time_ += lwp::GetDeltaTime();
}

void Player::UpdateMove()
{
	if (moveData_->maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / moveData_->maxTime_;
	//moveData_->time_ += lwp::GetDeltaTime();
	// 移動距離とモーション用の更新
	//t = (moveData_->time_ / (float)moveData_->maxTime_);
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
	moveVector.y = 0.0f;

	// モデル回転
	demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	//moveVector = moveVector.Normalize() * cSPEEDMOVE_ * max(min(t / moveData_->maxTime_, 1.0f), 0.0f);
	moveVector = moveVector.Normalize() * (cSPEEDMOVE_ / moveData_->maxTime_) * lwp::GetDeltaTime();

	demoModel_->transform.translation += moveVector;
}

void Player::UpdateSlash()
{
	// 無敵時間
	// ジャスト成立中
	//　無敵時間中
	playerCollision_->isActive = (!isJustSlashing_ && cTIMEJUSTSLASH_ + cTIMEADDINCVINCIBLE_ < t);
	// 判定を取れるようにする
	justCollision_->isActive = t < cTIMEJUSTSLASH_;
	// 武器の判定を伸ばす
	weaponCollision_->end = demoModel_->transform.translation + slashData_->vector_ * cPLUSWEAPONCORRECTION_;
	if (slashData_->maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Moment;
	}
	easeT_ = t / slashData_->maxTime_;
	//slashData_->time_ += lwp::GetDeltaTime();
	// 移動距離とモーション用の更新
	//t = (slashData_->time_ / (float)slashData_->maxTime_);

	//lwp::Vector3 moveVector = slashData_->vector_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
	lwp::Vector3 moveVector = slashData_->vector_;

	// モデル回転
	demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	//moveVector = moveVector.Normalize() * cSPEEDSLASH_ * max(min(t / slashData_->maxTime_, 1.0f), 0.0f);
	moveVector = moveVector * (cSPEEDSLASH_ / slashData_->maxTime_) * lwp::GetDeltaTime();

	demoModel_->transform.translation += moveVector;
}

void Player::UpdateMoment()
{
	if (momentData_->maxTime_ <= t)
	{
		reqBehavior_ = Behavior::Root;
	}
	easeT_ = t / momentData_->maxTime_;
	//momentData_->time_ += lwp::GetDeltaTime();
	// 移動距離とモーション用の更新
	//t = (momentData_->time_ / (float)momentData_->maxTime_);
	if (isInputMove_)
	{
		lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
		moveVector.y = 0.0f;

		// モデル回転
		demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

		//moveVector = moveVector.Normalize() * cSPEEDSLASH_ * 0.01f * max(min(t / momentData_->maxTime_, 1.0f), 0.0f);
		moveVector = moveVector.Normalize() * (cSPEEDMOMENT_ / momentData_->maxTime_) * lwp::GetDeltaTime();

		demoModel_->transform.translation += moveVector;
	}
}

void Player::InitDatas()
{
	// 状態を初期状態に設定
	behavior_ = Behavior::Root;

	// 状態を設定
	rootData_.reset(InitRootData());
	moveData_.reset(InitMoveData());
	slashData_.reset(InitSlashData());
	momentData_.reset(InitMomentData());


	// 今の状態を設定
	//currentData_ = behaviorDatas_[static_cast<size_t>(behavior_)].get();
}

Player::RootData* Player::InitRootData()
{
	RootData* data = new RootData;
	data->cBASETIME = 0.5f;
	//data->time_ = 0.0f;
	data->maxTime_ = 0.0f;
	return data;
}

Player::MoveData* Player::InitMoveData()
{
	MoveData* data = new MoveData;
	data->cBASETIME = 0.1f;
	//data->time_ = 0.0f;
	data->maxTime_ = 0.0f;
	return data;
}

Player::SlashData* Player::InitSlashData()
{
	SlashData* data = new SlashData;
	data->cBASETIME = 0.25f;
	data->vector_ = { 0.0f,0.0f,1.0f };
	//data->time_ = 0.0f;
	data->maxTime_ = 0.0f;
	data->relationSlash_ = 0u;
	data->cMAXRELATION_ = 3u;
	data->maxRelation_ = 0u;
	return data;
}

Player::MomentData* Player::InitMomentData()
{
	MomentData* data = new MomentData;
	data->cBASETIME = 0.5f;
	//data->time_ = 0.0f;
	data->maxTime_ = 0.0f;
	data->relationSlash_ = 0u;
	return data;
}

void Player::CreateCollision()
{
	// 当たり判定を設定
	playerCollision_ = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	// 武器との当たり判定を取る
	playerCollision_->CreateFromPrimitive(demoModel_);
	// マスク
	playerCollision_->mask.SetBelongFrag(MaskLayer::Player);
	// 敵または敵の攻撃
	playerCollision_->mask.SetHitFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	// チョットした後隙
	playerCollision_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		if (data.state == OnCollisionState::Trigger)
		{
			reqBehavior_ = Behavior::Moment;
			EndJust();
		}
		});
	playerCollision_->isActive = true;
	// 当たり判定を設定
	weaponCollision_ = LWP::Common::CreateInstance<lwp::Collider::Capsule>();
	// 武器との当たり判定を取る
	weaponCollision_->Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	weaponCollision_->radius = cRADIUSWEAPONCOLLISION_;
	// マスク
	weaponCollision_->mask.SetBelongFrag(MaskLayer::Layer3);
	weaponCollision_->mask.SetHitFrag(MaskLayer::Enemy);
	// 今のところは何もしていない
	weaponCollision_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		});
	weaponCollision_->isActive = false;

	CreateJustCollision();
}

void Player::CreateJustCollision()
{
	// ジャスト居合
	justCollision_ = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	justCollision_->Create(demoModel_->transform.translation);
	// サイズ
	//justCollision_->max = playerCollision_->max + lwp::Vector3(1.0f, 1.0f, 1.0f);
	//justCollision_->min = playerCollision_->min - lwp::Vector3(1.0f, 1.0f, 1.0f);
	// マスク
	justCollision_->mask.SetBelongFrag(MaskLayer::Player);
	justCollision_->mask.SetHitFrag(MaskLayer::Layer2);
	// ジャスト居合したことを通知
	justCollision_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		if (!(data.state == OnCollisionState::None) &&
			data.hit &&
			(data.hit->mask.GetBelongFrag() & MaskLayer::Layer2))
		{
			TItleScene* const scene = dynamic_cast<TItleScene*>(pScene_);
			assert(scene);
			scene->StartJustSlash();
			isJustSlashing_ = true;
			pCamera_->ReduceFov();
			if (slashData_->maxRelation_ <= slashData_->cMAXRELATION_)
			{
				slashData_->maxRelation_++;
				slashPanel_->Just();
			}
		}
		});
	// フラグオフ
	justCollision_->isActive = false;

#ifdef DEMO
	justCollision_->name = "Just";
#endif
}

void Player::UpdateInput()
{
	// コマンドを積み重ねたものを取得
	pCommands_ = pInput_->HandleInput();

	// クリア
	commands_.clear();
	isInputMove_ = false;

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
		isInputMove_ = true;
	}
	if ((destinate_.z < 0 ? -destinate_.z : destinate_.z)
		< (y < 0 ? -y : y))
	{
		destinate_.z = y;
		commands_.push_back(Behavior::Move);
		// 移動キーが入力されている時通る
		isInputMove_ = true;
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
	// コマンドの初期化
	command_ = nullptr;

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
			if ((behavior_ != Behavior::Slash || isJustSlashing_) &&
				slashData_->relationSlash_ < slashData_->maxRelation_)
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
		ImGui::Text("BaseFrame : %.3f", rootData_->cBASETIME);
		ImGui::Text("MaxFrame  : %.3f", rootData_->maxTime_);
		//ImGui::Text("Frame     : %d", rootData_->time_);
		break;
	case Player::Behavior::Move:
		ImGui::Text("MOVE");
		ImGui::Text("BaseFrame : %.3f", moveData_->cBASETIME);
		ImGui::Text("MaxFrame  : %.3f", moveData_->maxTime_);
		//ImGui::Text("Frame     : %d", moveData_->time_);
		break;
	case Player::Behavior::Slash:
		ImGui::Text("SLASH");
		ImGui::Text("BaseFrame : %.3f", slashData_->cBASETIME);
		ImGui::Text("MaxFrame  : %.3f", slashData_->maxTime_);
		//ImGui::Text("Frame     : %d", slashData_->time_);
		break;
	case Player::Behavior::Moment:
		ImGui::Text("MOMENT");
		ImGui::Text("BaseFrame : %.3f", momentData_->cBASETIME);
		ImGui::Text("MaxFrame  : %.3f", momentData_->maxTime_);
		//ImGui::Text("Frame     : %d", momentData_->time_);
		break;
	default:
		break;
	}

	ImGui::Text("t : %.3f", t);

	ImGui::Separator();

	ImGui::Text("SlashRelation / MaxRelation");
	ImGui::Text("%d / %d", slashData_->relationSlash_, slashData_->maxRelation_);
	ImGui::Text("INCREMENTMOMENT : %.3f", cTIMEINCREMENTMOMENT_);

	ImGui::End();
}
