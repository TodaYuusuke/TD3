#include "Player.h"

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
}

void Player::UpdateInput()
{
	pCommand_ = pInput_->HandleInput();

	if (pCommand_)
	{
		pCommand_->Exec(*this);
		reqBehavior_ = Behavior::Move;
	}
	// コントローラーの入力を合わせる
	float x = LWP::Input::Controller::GetLStick().x;
	float y = LWP::Input::Controller::GetLStick().y;
	destinate_.x += x;
	destinate_.z += y;
	destinate_ = destinate_.Normalize();
}

void Player::Update()
{
	UpdateInput();
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case Player::Behavior::Root:

			break;
		case Player::Behavior::Move:

			break;
		case Player::Behavior::_COUNT:
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
	case Player::Behavior::_COUNT:
		break;
	default:
		break;
	}
}

void Player::MoveFront()
{
	// 向いている方向へ変換するので単純にしている
	destinate_.z += 1.0f;
}

void Player::MoveBack()
{
	destinate_.z -= 1.0f;
}

void Player::MoveLeft()
{
	destinate_.x -= 1.0f;
}

void Player::MoveRight()
{
	destinate_.x += 1.0f;
}

void Player::UpdateRoot()
{
}

void Player::UpdateMove()
{
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = destinate_ * lwp::Matrix4x4::CreateRotateXYZMatrix(camera_->transform.rotation);
	moveVector.y = 0.0f;
	moveVector = moveVector.Normalize() * cPLAYERSPEED_ * lwp::GetDeltaTime();

	world_.translation += moveVector;
	destinate_ = { 0.0,0.0,0.0 };
	reqBehavior_ = Behavior::Root;
}
