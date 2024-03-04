#pragma once

#include <list>

#include "PlayerCommand.h"

class PlayerInput
{
public:
	// 返すコマンド群
	std::list<IPlayerCommand*>* HandleInput();

	// コマンドの配置
	void AssignCommands();

private:

	// 入力を取る
	void HandleInputKey();
	void HandleInputPad();

	// コマンドを配置
	void AssignCommandToKey();
	void AssignCommandToPad();

	//*** キーボード入力 ***//

	void AssignCommandToPressKeyW();
	void AssignCommandToPressKeyS();
	void AssignCommandToPressKeyA();
	void AssignCommandToPressKeyD();

	void AssignCommandToTriggerSPACE();

	//*** パッド入力 ***//

	void AssginCommandToPressPadA();
	void AssginCommandToPressPadB();
	void AssginCommandToPressPadX();
	void AssginCommandToPressPadY();

private:

	// コマンドを積み重ねる
	std::list<IPlayerCommand*> commands_;

	//*** キーボード入力 ***//

	IPlayerCommand* pressKeyW_;
	IPlayerCommand* pressKeyA_;
	IPlayerCommand* pressKeyS_;
	IPlayerCommand* pressKeyD_;

	IPlayerCommand* triggerKeySPACE_;

	//*** パッド入力 ***//

	IPlayerCommand* pressPadA_;
	IPlayerCommand* pressPadB_;
	IPlayerCommand* pressPadX_;
	IPlayerCommand* pressPadY_;

};

