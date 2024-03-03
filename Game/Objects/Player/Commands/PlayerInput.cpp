#include "PlayerInput.h"

#include "Adapter.h"

IPlayerCommand* PlayerInput::HandleInput()
{
	if (LWP::Input::Keyboard::GetPress(DIK_W))
	{
		return pressKeyW_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_S))
	{
		return pressKeyS_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_A))
	{
		return pressKeyA_;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_D))
	{
		return pressKeyD_;
	}

	// 居合攻撃
	if (LWP::Input::Keyboard::GetTrigger(DIK_SPACE))
	{
		return triggerKeySPACE_;
	}

	return nullptr;
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
