#pragma once

#include <list>

#include "PlayerCommand.h"

class PlayerInput
{
public:
	std::list<IPlayerCommand*>* HandleInput();

	void AssignMoveCommandToPressKey();

private:

	void AssignCommandToPressKeyW();
	void AssignCommandToPressKeyS();
	void AssignCommandToPressKeyA();
	void AssignCommandToPressKeyD();

	void AssignCommandToTriggerSPACE();

private:

	// コマンドを積み重ねる
	std::list<IPlayerCommand*> commands_;

	IPlayerCommand* pressKeyW_;
	IPlayerCommand* pressKeyA_;
	IPlayerCommand* pressKeyS_;
	IPlayerCommand* pressKeyD_;

	IPlayerCommand* triggerKeySPACE_;
};

