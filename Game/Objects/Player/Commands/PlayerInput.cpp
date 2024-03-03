#include "PlayerInput.h"

#include "Adapter.h"

std::list<IPlayerCommand*>* PlayerInput::HandleInput()
{
	commands_.clear();

	// 入力的にこっちが早い方がいい?
	// 居合攻撃
	if (LWP::Input::Keyboard::GetPress(DIK_SPACE))
	{
		commands_.push_back(triggerKeySPACE_);
		//return triggerKeySPACE_;
	}
	// 移動
	if (LWP::Input::Keyboard::GetPress(DIK_W))
	{
		commands_.push_back(pressKeyW_);
		//return pressKeyW_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_S))
	{
		commands_.push_back(pressKeyS_);
		//return pressKeyS_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_A))
	{
		commands_.push_back(pressKeyA_);
		//return pressKeyA_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_D))
	{
		commands_.push_back(pressKeyD_);
		//return pressKeyD_;
	}


	return &commands_;
}

void PlayerInput::AssignMoveCommandToPressKey()
{
	AssignCommandToPressKeyW();
	AssignCommandToPressKeyS();
	AssignCommandToPressKeyA();
	AssignCommandToPressKeyD();

	AssignCommandToTriggerSPACE();
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
