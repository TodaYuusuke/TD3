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

	return nullptr;
}

void PlayerInput::AssignMoveCommandToPressKey()
{
	AssinngCommandToPressKeyW();
	AssinngCommandToPressKeyS();
	AssinngCommandToPressKeyA();
	AssinngCommandToPressKeyD();
}

void PlayerInput::AssinngCommandToPressKeyW()
{
	IPlayerCommand* command = new PlayerMoveFrontCommand();
	this->pressKeyW_ = command;
}

void PlayerInput::AssinngCommandToPressKeyS()
{
	IPlayerCommand* command = new PlayerMoveBackCommand();
	this->pressKeyS_ = command;
}

void PlayerInput::AssinngCommandToPressKeyA()
{
	IPlayerCommand* command = new PlayerMoveLeftCommand();
	this->pressKeyA_ = command;
}

void PlayerInput::AssinngCommandToPressKeyD()
{
	IPlayerCommand* command = new PlayerMoveRightCommand();
	this->pressKeyD_ = command;
}
