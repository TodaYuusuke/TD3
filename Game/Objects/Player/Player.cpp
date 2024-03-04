#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete pInput_;
}

void Player::Initialize()
{
	world_.Initialize();
	// モデル読み込み
	demoModel = LWP::Resource::LoadModel("cube/cube.obj");
	demoModel->transform.Parent(&world_);
	demoModel->isActive = true;
	demoModel->name = "Player";

	// 入力ハンドルを初期化
	pInput_ = new PlayerInput();
	pInput_->AssignCommands();

	// 状態の情報を設定
	InitDatas();

}

void Player::Update()
{
	DebugWindow();

	UpdateInput();

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		//currentData_ = behaviorDatas_[static_cast<size_t>(behavior_)].get();
		//currentData_->frame_ = 0;
		switch (behavior_)
		{
		case Player::Behavior::Root:
			rootData_->frame_ = 0;
			break;
		case Player::Behavior::Move:
			moveData_->frame_ = 0;
			break;
		case Player::Behavior::Slash:
			slashData_->frame_ = 0;
			slashData_->vector_ = destinate_;
			break;
		case Player::Behavior::Moment:
			momentData_->frame_ = 0;
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
}

void Player::MoveFront()
{
	// 向いている方向へ変換するので単純にしている
	destinate_.z = 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
}

void Player::MoveBack()
{
	destinate_.z = -1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
}

void Player::MoveLeft()
{
	destinate_.x = -1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
}

void Player::MoveRight()
{
	destinate_.x = 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Behavior::Move);
}

void Player::Slash()
{
	//destinate_.z += 1.0f;
	//reqBehavior_ = Behavior::Slash;
	commands_.push_back(Behavior::Slash);
}

void Player::UpdateRoot()
{
	if (rootData_->cMAXFRAME <= rootData_->frame_)
	{
		reqBehavior_ = Behavior::Root;
	}
	rootData_->frame_++;
}

void Player::UpdateMove()
{
	if (moveData_->cMAXFRAME <= moveData_->frame_)
	{
		//destinate_ = { 0.0,0.0,0.0 };
		reqBehavior_ = Behavior::Root;
	}
	moveData_->frame_++;
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cMOVESPEED_ * (float)lwp::GetDeltaTime();

	world_.translation += moveVector;
}


void Player::UpdateSlash()
{
	if (slashData_->cMAXFRAME <= slashData_->frame_)
	{
		//destinate_ = { 0.0,0.0,0.0 };
		reqBehavior_ = Behavior::Moment;
	}
	slashData_->frame_++;

	lwp::Vector3 moveVector = slashData_->vector_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cSLASHSPEED_ * (float)lwp::GetDeltaTime();

	world_.translation += moveVector;
}

void Player::UpdateMoment()
{
	if (momentData_->cMAXFRAME <= momentData_->frame_)
	{
		//destinate_ = { 0.0,0.0,0.0 };
		reqBehavior_ = Behavior::Root;
	}
	momentData_->frame_++;
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cSLASHSPEED_ * 0.01f * (float)lwp::GetDeltaTime();

	world_.translation += moveVector;
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
	data->cMAXFRAME = 1;
	return data;
}

Player::MoveData* Player::InitMoveData()
{
	MoveData* data = new MoveData;
	data->cMAXFRAME = 5;
	return data;
}

Player::SlashData* Player::InitSlashData()
{
	SlashData* data = new SlashData;
	data->cMAXFRAME = 10;

	return data;
}

Player::MomentData* Player::InitMomentData()
{
	MomentData* data = new MomentData;
	data->cMAXFRAME = 20;

	return data;
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
	// ここもコマンドにしたい--
	// 移動方向のみなので円状の Vector2 を使いたい
	float x = LWP::Input::Controller::GetLStick().x;
	float y = LWP::Input::Controller::GetLStick().y;
	if ((destinate_.x < 0 ? -destinate_.x : destinate_.x)
		< (x < 0 ? -x : x))
	{
		destinate_.x = x;
		commands_.push_back(Behavior::Move);
	}
	if ((destinate_.z < 0 ? -destinate_.z : destinate_.z)
		< (y < 0 ? -y : y))
	{
		destinate_.z = y;
		commands_.push_back(Behavior::Move);
	}
	destinate_ = destinate_.Normalize();

	// 方向がゼロだった場合は元に戻す
	if (destinate_.x == 0 && destinate_.z == 0)
	{
		destinate_ = direct.Normalize();
	}

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
			if (behavior_ == Behavior::Root)
			{
				reqBehavior_ = Behavior::Move;
			}
			break;
		case Behavior::Slash:
			if (behavior_ != Behavior::Slash &&
				behavior_ != Behavior::Moment)
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

	ImGui::End();
}
