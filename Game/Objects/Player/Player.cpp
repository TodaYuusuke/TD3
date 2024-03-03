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
	pInput_->AssignMoveCommandToPressKey();

	// 状態の情報を設定
	InitDatas();

}

void Player::Update()
{
	UpdateInput();

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		currentData_ = behaviorDatas_[static_cast<size_t>(behavior_)].get();
		currentData_->frame_ = 0;
		switch (behavior_)
		{
		case Player::Behavior::Root:

			break;
		case Player::Behavior::Move:

			break;
		case Player::Behavior::Slash:
			break;
		case Player::Behavior::Moment:
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
	destinate_.z += 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Command::toMoveF);
}

void Player::MoveBack()
{
	destinate_.z -= 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Command::toMoveB);
}

void Player::MoveLeft()
{
	destinate_.x -= 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Command::toMoveL);
}

void Player::MoveRight()
{
	destinate_.x += 1.0f;
	//reqBehavior_ = Behavior::Move;
	commands_.push_back(Command::toMoveR);
}

void Player::Slash()
{
	//destinate_.z += 1.0f;
	//reqBehavior_ = Behavior::Slash;
  	commands_.push_back(Command::toSlash);
}

void Player::UpdateRoot()
{
	if (currentData_->cMAXFRAME <= currentData_->frame_)
	{
		reqBehavior_ = Behavior::Root;
	}
	currentData_->frame_++;
}

void Player::UpdateMove()
{
	if (currentData_->cMAXFRAME <= currentData_->frame_)
	{
		//destinate_ = { 0.0,0.0,0.0 };
		reqBehavior_ = Behavior::Root;
	}
	currentData_->frame_++;
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cMOVESPEED_ * lwp::GetDeltaTime();

	world_.translation += moveVector;
}


void Player::UpdateSlash()
{
	if (currentData_->cMAXFRAME <= currentData_->frame_)
	{
		//destinate_ = { 0.0,0.0,0.0 };
		reqBehavior_ = Behavior::Moment;
	}
	currentData_->frame_++;

	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cSLASHSPEED_ * lwp::GetDeltaTime();

	world_.translation += moveVector;
}

void Player::UpdateMoment()
{
	if (currentData_->cMAXFRAME <= currentData_->frame_)
	{
		//destinate_ = { 0.0,0.0,0.0 };
		reqBehavior_ = Behavior::Root;
	}
	currentData_->frame_++;
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cSLASHSPEED_ * 0.01f * lwp::GetDeltaTime();

	world_.translation += moveVector;
}

void Player::InitDatas()
{
	// 状態を初期状態に設定
	behavior_ = Behavior::Root;

	// 状態を設定
	behaviorDatas_[static_cast<size_t>(Behavior::Root)].reset(InitRootData());
	behaviorDatas_[static_cast<size_t>(Behavior::Move)].reset(InitMoveData());
	behaviorDatas_[static_cast<size_t>(Behavior::Slash)].reset(InitSlashData());
	behaviorDatas_[static_cast<size_t>(Behavior::Moment)].reset(InitMomentData());

	// 今の状態を設定
	currentData_ = behaviorDatas_[static_cast<size_t>(behavior_)].get();
}

Player::BaseData* Player::InitRootData()
{
	BaseData* data = new RootData;
	data->cMAXFRAME = 1;
	return data;
}

Player::BaseData* Player::InitMoveData()
{
	BaseData* data = new MoveData;
	data->cMAXFRAME = 5;
	return data;
}

Player::BaseData* Player::InitSlashData()
{
	BaseData* data = new SlashData;
	data->cMAXFRAME = 10;

	return data;
}

Player::BaseData* Player::InitMomentData()
{
	BaseData* data = new MomentData;
	data->cMAXFRAME = 20;

	return data;
}

void Player::UpdateInput()
{
	// 何か行動中だったら処理しない
	// ダメージ中は処理変えたい
	if (behavior_ != Behavior::Root)
	{
		return;
	}

	// コマンドを積み重ねる
	pCommands_ = pInput_->HandleInput();

	if (pCommands_->empty())
	{
		return;
	}
	// クリア
	commands_.clear();

	// コマンドを実行
	// 実際には情報を一度すべて受け取る
	for (std::list<IPlayerCommand*>::iterator itr = pCommands_->begin();
		itr != pCommands_->end(); ++itr)
	{
		(*itr)->Exec(*this);
	}

	// コントローラーの入力を合わせる
	// ここもコマンドにしたい
	// 移動方向のみなので円状の Vector2 を使いたい
	float x = LWP::Input::Controller::GetLStick().x;
	float y = LWP::Input::Controller::GetLStick().y;
	destinate_.x += x;
	destinate_.z += y;
	destinate_ = destinate_.Normalize();

	// コマンドの初期化
	command_ = nullptr;

	// 積み重ねたコマンドから実際の行動を決定する
	for (std::list<Command>::iterator itr = commands_.begin();
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
		case Command::toMoveF:
		case Command::toMoveB:
		case Command::toMoveL:
		case Command::toMoveR:
			reqBehavior_ = Behavior::Move;
			break;
		case Command::toSlash:
			reqBehavior_ = Behavior::Slash;
			break;
		}
	}

}