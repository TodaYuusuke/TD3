#pragma once

#include "PlayerCommand.h"

class PlayerInput
{
public:
	IPlayerCommand* HandleInput();

	void AssignMoveCommandToPressKey();

private:

	void AssinngCommandToPressKeyW();
	void AssinngCommandToPressKeyS();
	void AssinngCommandToPressKeyA();
	void AssinngCommandToPressKeyD();

private:
	IPlayerCommand* pressKeyW_;
	IPlayerCommand* pressKeyA_;
	IPlayerCommand* pressKeyS_;
	IPlayerCommand* pressKeyD_;

};

