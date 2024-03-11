#include "Player.h"

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
	weapon_->SetTPointer(&t);

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
	// デバッグ表示
	DebugWindow();

	// コマンド動作を確認
	UpdateInput();

	// 状態を変えるか判別
	CheckBehavior();

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		t = 0.0f;
		//currentData_ = behaviorDatas_[static_cast<size_t>(behavior_)].get();
		//currentData_->frame_ = 0;
		switch (behavior_)
		{
		case Player::Behavior::Root:
			rootData_->frame_ = 0;
			rootData_->maxFrame_ = rootData_->cBASEFRAME;
			// 居合回数のリセット
			slashData_->relationSlash_ = 0;
			weapon_->SetBehavior(Weapon::Behavior::Root);
			// UI に反映
			slashPanel_->Reset();
			break;
		case Player::Behavior::Move:
			moveData_->frame_ = 0;
			moveData_->maxFrame_ = moveData_->cBASEFRAME;
			break;
		case Player::Behavior::Slash:
			slashData_->frame_ = 0;
			slashData_->vector_ = destinate_;
			slashData_->maxFrame_ = slashData_->cBASEFRAME;
			weapon_->SetBehavior(Weapon::Behavior::Slash);
			// UI に反映
			slashPanel_->Slash();
			playerCollision_->isActive = true;
			weaponCollision_->isActive = true;
			break;
		case Player::Behavior::Moment:
			momentData_->frame_ = 0;
			momentData_->relationSlash_ = slashData_->relationSlash_;
			// 回数分フレームを加算
			momentData_->maxFrame_ = momentData_->cBASEFRAME + (momentData_->relationSlash_ * cFRAMEINCREMENTMOMENT_);
			// 居合回数加算
			slashData_->relationSlash_++;
			weapon_->SetBehavior(Weapon::Behavior::Moment);
			//playerCollision_->isActive = false;
			//weaponCollision_->isActive = false;
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

	weapon_->Update();
	slashPanel_->Update();
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
	if (rootData_->maxFrame_ <= rootData_->frame_)
	{
		reqBehavior_ = Behavior::Root;
	}
	rootData_->frame_++;
}

void Player::UpdateMove()
{
	if (moveData_->maxFrame_ <= moveData_->frame_)
	{
		reqBehavior_ = Behavior::Root;
	}
	moveData_->frame_++;
	// 移動距離とモーション用の更新
	t = (moveData_->frame_ / (float)moveData_->maxFrame_);
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
	moveVector.y = 0.0f;

	// モデル回転
	demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);
	
	moveVector = moveVector.Normalize() * cSPEEDMOVE_ * (float)lwp::GetDeltaTime();

	demoModel_->transform.translation += moveVector;
}


void Player::UpdateSlash()
{
	if (slashData_->maxFrame_ <= slashData_->frame_)
	{
		reqBehavior_ = Behavior::Moment;
	}
	slashData_->frame_++;
	// 移動距離とモーション用の更新
	t = (slashData_->frame_ / (float)slashData_->maxFrame_);

	lwp::Vector3 moveVector = slashData_->vector_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
	moveVector.y = 0.0f;

	// モデル回転
	demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	moveVector = moveVector.Normalize() * cSPEEDSLASH_ * (float)lwp::GetDeltaTime();

	demoModel_->transform.translation += moveVector;
}

void Player::UpdateMoment()
{
	if (momentData_->maxFrame_ <= momentData_->frame_)
	{
		reqBehavior_ = Behavior::Root;
	}
	momentData_->frame_++;
	// 移動距離とモーション用の更新
	t = (momentData_->frame_ / (float)momentData_->maxFrame_);
	if (isInputMove_)
	{
		lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
		moveVector.y = 0.0f;

		// モデル回転
		demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

		moveVector = moveVector.Normalize() * cSPEEDSLASH_ * 0.01f * (float)lwp::GetDeltaTime();

		demoModel_->transform.translation += moveVector;
	}
}

void Player::InitDatas()
{
	// 状態を初期状態に設定
	behavior_ = Behavior::Root;

	// 状態を設定
	//behaviorDatas_[static_cast<size_t>(Behavior::Root)].reset(InitRootData());
	//behaviorDatas_[static_cast<size_t>(Behavior::Move)].reset(InitMoveData());
	//behaviorDatas_[static_cast<size_t>(Behavior::Slash)].reset(InitSlashData());
	//behaviorDatas_[static_cast<size_t>(Behavior::Moment)].reset(InitMomentData());
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
	data->cBASEFRAME = 1;
	data->frame_ = 0;
	data->maxFrame_ = 0;
	return data;
}

Player::MoveData* Player::InitMoveData()
{
	MoveData* data = new MoveData;
	data->cBASEFRAME = 5;
	data->frame_ = 0;
	data->maxFrame_ = 0;
	return data;
}

Player::SlashData* Player::InitSlashData()
{
	SlashData* data = new SlashData;
	data->cBASEFRAME = 10;
	data->vector_ = { 0.0f,0.0f,1.0f };
	data->frame_ = 0;
	data->maxFrame_ = 0;
	data->relationSlash_ = 0;
	data->MaxRelation_ = 3;
	return data;
}

Player::MomentData* Player::InitMomentData()
{
	MomentData* data = new MomentData;
	data->cBASEFRAME = 30;
	data->frame_ = 0;
	data->maxFrame_ = 0;
	data->relationSlash_ = 0;
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
	playerCollision_->mask.SetHitFrag(MaskLayer::Enemy);
	// 今のところは何もしていない
	playerCollision_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		});
	playerCollision_->isActive = false;
	// 当たり判定を設定
	weaponCollision_ = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	// 武器との当たり判定を取る
	weaponCollision_->CreateFromPrimitive(weapon_->GetMesh());
	// マスク
	weaponCollision_->mask.SetBelongFrag(MaskLayer::Player | MaskLayer::Layer2);
	weaponCollision_->mask.SetHitFrag(MaskLayer::Enemy);
	// 今のところは何もしていない
	weaponCollision_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		});
	weaponCollision_->isActive = false;

	// ジャスト居合
	justCollision_ = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	justCollision_->CreateFromPrimitive(demoModel_);
	// マスク
	justCollision_->mask.SetBelongFrag(MaskLayer::Player);
	justCollision_->mask.SetHitFrag(MaskLayer::Enemy & MaskLayer::Layer2);
	// 今のところは何もしていない
	justCollision_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		data;
		});
	// フラグオフ
	justCollision_->isActive = false;
}

void Player::UpdateInput()
{
	// コマンドを積み重ねたものを取得
	pCommands_ = pInput_->HandleInput();

	// 操作が無いなら終了
	/*if (pCommands_->empty())
	{
		return;
	}*/
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
			if (behavior_ != Behavior::Slash &&
				slashData_->relationSlash_ < slashData_->MaxRelation_)
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
		ImGui::Text("BaseFrame : %d", rootData_->cBASEFRAME);
		ImGui::Text("MaxFrame  : %d", rootData_->maxFrame_);
		ImGui::Text("Frame     : %d", rootData_->frame_);
		break;
	case Player::Behavior::Move:
		ImGui::Text("MOVE");
		ImGui::Text("BaseFrame : %d", moveData_->cBASEFRAME);
		ImGui::Text("MaxFrame  : %d", moveData_->maxFrame_);
		ImGui::Text("Frame     : %d", moveData_->frame_);
		break;
	case Player::Behavior::Slash:
		ImGui::Text("SLASH");
		ImGui::Text("BaseFrame : %d", slashData_->cBASEFRAME);
		ImGui::Text("MaxFrame  : %d", slashData_->maxFrame_);
		ImGui::Text("Frame     : %d", slashData_->frame_);
		break;
	case Player::Behavior::Moment:
		ImGui::Text("MOMENT");
		ImGui::Text("BaseFrame : %d", momentData_->cBASEFRAME);
		ImGui::Text("MaxFrame  : %d", momentData_->maxFrame_);
		ImGui::Text("Frame     : %d", momentData_->frame_);
		break;
	default:
		break;
	}

	ImGui::Separator();

	ImGui::Text("SlashRelation : %d", slashData_->relationSlash_);

	ImGui::End();
}
