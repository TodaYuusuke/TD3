#pragma once

#include "PlayerCommand.h"

class PlayerInput
{
public:
	IPlayerCommand* HandleInput();

	void AssignMoveCommandToPressKey();

private:

	void AssignCommandToPressKeyW();
	void AssignCommandToPressKeyS();
	void AssignCommandToPressKeyA();
	void AssignCommandToPressKeyD();

	void AssignCommandToTriggerSPACE();

private:
	IPlayerCommand* pressKeyW_;
	IPlayerCommand* pressKeyA_;
	IPlayerCommand* pressKeyS_;
	IPlayerCommand* pressKeyD_;

	IPlayerCommand* triggerKeySPACE_;
};

