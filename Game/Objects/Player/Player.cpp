#include "Player.h"

Player::~Player()
{
	delete pInput_;
}

void Player::Initialize()
{
	world_.Initialize();
	// ���f���ǂݍ���
	demoModel = LWP::Resource::LoadModel("cube/cube.obj");
	demoModel->transform.Parent(&world_);
	demoModel->transform.scale = { 5.0f,5.0f, 0.05f };
	demoModel->isActive = true;
	demoModel->name = "Player";

	// ���̓n���h����������
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
	// �����Ă�������֕ϊ�����̂ŒP���ɂ��Ă���
	destinate_.z += cPLAYERSPEED_;
}

void Player::MoveBack()
{
	destinate_.z -= cPLAYERSPEED_;
}

void Player::MoveLeft()
{
	destinate_.x -= cPLAYERSPEED_;
}

void Player::MoveRight()
{
	destinate_.x += cPLAYERSPEED_;
}

void Player::UpdateRoot()
{
}

void Player::UpdateMove()
{
	world_.translation += destinate_;
}
