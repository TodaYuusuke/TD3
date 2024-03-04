#include "PlayerInput.h"

#include "Adapter.h"

std::list<IPlayerCommand*>* PlayerInput::HandleInput()
{
	commands_.clear();

	HandleInputKey();
	HandleInputPad();

	return &commands_;
}

void PlayerInput::AssignCommands()
{
	AssignCommandToKey();
	AssignCommandToPad();
}

void PlayerInput::HandleInputKey()
{
	// 入力的にこっちが早い方がいい? ← 関係なくなった
	// 居合攻撃
	if (LWP::Input::Keyboard::GetTrigger(DIK_SPACE))
	{
		commands_.push_back(triggerKeySPACE_);
	}
	// 移動
	// 相反する入力を受け付けない
	if (LWP::Input::Keyboard::GetPress(DIK_W))
	{
		commands_.push_back(pressKeyW_);
	}
	else if (LWP::Input::Keyboard::GetPress(DIK_S))
	{
		commands_.push_back(pressKeyS_);
	}
	if (LWP::Input::Keyboard::GetPress(DIK_A))
	{
		commands_.push_back(pressKeyA_);
	}
	else if (LWP::Input::Keyboard::GetPress(DIK_D))
	{
		commands_.push_back(pressKeyD_);
	}
}

void PlayerInput::HandleInputPad()
{
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_A))
	{
		commands_.push_back(pressPadA_);
	}
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_B))
	{
		commands_.push_back(pressPadB_);
	}
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_X))
	{
		commands_.push_back(pressPadX_);
	}
	if (LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_Y))
	{
		commands_.push_back(pressPadY_);
	}
}

void PlayerInput::AssignCommandToKey()
{
	AssignCommandToPressKeyW();
	AssignCommandToPressKeyS();
	AssignCommandToPressKeyA();
	AssignCommandToPressKeyD();

	AssignCommandToTriggerSPACE();
}

void PlayerInput::AssignCommandToPad()
{
	AssginCommandToPressPadA();
	AssginCommandToPressPadB();
	AssginCommandToPressPadX();
	AssginCommandToPressPadY();
}

void PlayerInput::AssignCommandToPressKeyW()
{
	IPlayerCommand* command = new PlayerMoveFrontCommand();
	this->pressKeyW_ = command;
}

void PlayerInput::AssignCommandToPressKeyS()
{
	IPlayerCommand* command = new PlayerMoveBackCommand();
	this->pressKeyS_ = command;
}

void PlayerInput::AssignCommandToPressKeyA()
{
	IPlayerCommand* command = new PlayerMoveLeftCommand();
	this->pressKeyA_ = command;
}

void PlayerInput::AssignCommandToPressKeyD()
{
	IPlayerCommand* command = new PlayerMoveRightCommand();
	this->pressKeyD_ = command;
}

void PlayerInput::AssignCommandToTriggerSPACE()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->triggerKeySPACE_ = command;
}



void PlayerInput::AssginCommandToPressPadA()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->pressPadA_ = command;
}

void PlayerInput::AssginCommandToPressPadB()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->pressPadB_ = command;
}

void PlayerInput::AssginCommandToPressPadX()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->pressPadX_ = command;
}

void PlayerInput::AssginCommandToPressPadY()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->pressPadY_ = command;
}
