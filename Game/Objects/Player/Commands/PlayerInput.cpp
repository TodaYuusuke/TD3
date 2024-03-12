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
	if (LWP::Input::Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		commands_.push_back(triggerPadA_);
	}
	if (LWP::Input::Pad::GetTrigger(XINPUT_GAMEPAD_B))
	{
		commands_.push_back(triggerPadB_);
	}
	if (LWP::Input::Pad::GetTrigger(XINPUT_GAMEPAD_X))
	{
		commands_.push_back(triggerPadX_);
	}
	if (LWP::Input::Pad::GetTrigger(XINPUT_GAMEPAD_Y))
	{
		commands_.push_back(triggerPadY_);
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
	AssginCommandToTriggerPadA();
	AssginCommandToTriggerPadB();
	AssginCommandToTriggerPadX();
	AssginCommandToTriggerPadY();
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



void PlayerInput::AssginCommandToTriggerPadA()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->triggerPadA_ = command;
}

void PlayerInput::AssginCommandToTriggerPadB()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->triggerPadB_ = command;
}

void PlayerInput::AssginCommandToTriggerPadX()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->triggerPadX_ = command;
}

void PlayerInput::AssginCommandToTriggerPadY()
{
	IPlayerCommand* command = new PlayerSlashCommand();
	this->triggerPadY_ = command;
}
