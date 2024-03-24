#pragma once

#include<list>

#include "UpgradeCommand.h"

class UpgradeInput
{
public:
	// 返すコマンド
	std::list<IUpgradeCommand*>* HandleInput();

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
	void AssignCommandToPressKeyUP();
	void AssignCommandToPressKeyDOWN();

	void AssignCommandToTriggerSPACE();
	void AssignCommandToTriggerENTER();

	//*** パッド入力 ***//

	void AssginCommandToTriggerPadA();
	void AssginCommandToTriggerPadB();
	void AssginCommandToTriggerPadX();
	void AssginCommandToTriggerPadY();

private:

	// コマンドを積み重ねる
	std::list<IUpgradeCommand*> commands_;

	//*** キーボード入力 ***//

	IUpgradeCommand* pressKeyW_;
	IUpgradeCommand* pressKeyS_;

	IUpgradeCommand* pressKeyUP_;
	IUpgradeCommand* pressKeyDOWN_;

	IUpgradeCommand* triggerKeySPACE_;
	IUpgradeCommand* triggerKeyENTER_;

	//*** パッド入力 ***//

	IUpgradeCommand* triggerPadA_;
	IUpgradeCommand* triggerPadB_;
	IUpgradeCommand* triggerPadX_;
	IUpgradeCommand* triggerPadY_;

};

