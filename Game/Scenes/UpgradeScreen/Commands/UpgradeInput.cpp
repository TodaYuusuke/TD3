#include "UpgradeInput.h"

#include <Adapter.h>

std::list<IUpgradeCommand*>* UpgradeInput::HandleInput()
{
	commands_.clear();

	HandleInputKey();
	HandleInputPad();

	return &commands_;
}

void UpgradeInput::AssignCommands()
{
	AssignCommandToKey();
	AssignCommandToPad();
}

void UpgradeInput::HandleInputKey()
{
	// 選択
	// 相反する入力を受け付けない
	if (LWP::Input::Keyboard::GetPress(DIK_W))
	{
		commands_.push_back(pressKeyW_);
	}
	else if (LWP::Input::Keyboard::GetPress(DIK_S))
	{
		commands_.push_back(pressKeyS_);
	}
	if (LWP::Input::Keyboard::GetPress(DIK_UP))
	{
		commands_.push_back(pressKeyUP_);
	}
	else if (LWP::Input::Keyboard::GetPress(DIK_DOWN))
	{
		commands_.push_back(pressKeyDOWN_);
	}

	// 決定
	if (LWP::Input::Keyboard::GetTrigger(DIK_SPACE))
	{
		commands_.push_back(triggerKeySPACE_);
	}
	if (LWP::Input::Keyboard::GetTrigger(DIK_RETURN))
	{
		commands_.push_back(triggerKeyENTER_);
	}
}

void UpgradeInput::HandleInputPad()
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

//*** コマンドの登録 ***//

void UpgradeInput::AssignCommandToKey()
{
	AssignCommandToPressKeyW();
	AssignCommandToPressKeyS();
	AssignCommandToPressKeyUP();
	AssignCommandToPressKeyDOWN();

	AssignCommandToTriggerSPACE();
	AssignCommandToTriggerENTER();
}

void UpgradeInput::AssignCommandToPad()
{
	AssginCommandToTriggerPadA();
	AssginCommandToTriggerPadB();
	AssginCommandToTriggerPadX();
	AssginCommandToTriggerPadY();
}

//*** 細かいキーの登録 ***//

void UpgradeInput::AssignCommandToPressKeyW()
{
	IUpgradeCommand* command = new UpgradeBackCommand();
	pressKeyW_ = command;
}

void UpgradeInput::AssignCommandToPressKeyS()
{
	IUpgradeCommand* command = new UpgradeNextCommand();
	pressKeyS_ = command;
}

void UpgradeInput::AssignCommandToPressKeyUP()
{
	IUpgradeCommand* command = new UpgradeBackCommand();
	pressKeyUP_ = command;
}

void UpgradeInput::AssignCommandToPressKeyDOWN()
{
	IUpgradeCommand* command = new UpgradeNextCommand();
	pressKeyDOWN_ = command;
}

void UpgradeInput::AssignCommandToTriggerSPACE()
{
	IUpgradeCommand* command = new UpgradeSelectCommand();
	triggerKeySPACE_ = command;
}

void UpgradeInput::AssignCommandToTriggerENTER()
{
	IUpgradeCommand* command = new UpgradeSelectCommand();
	triggerKeyENTER_ = command;
}

void UpgradeInput::AssginCommandToTriggerPadA()
{
	IUpgradeCommand* command = new UpgradeSelectCommand();
	triggerPadA_ = command;
}

void UpgradeInput::AssginCommandToTriggerPadB()
{
	IUpgradeCommand* command = new UpgradeSelectCommand();
	triggerPadB_ = command;
}

void UpgradeInput::AssginCommandToTriggerPadX()
{
	IUpgradeCommand* command = new UpgradeSelectCommand();
	triggerPadX_ = command;
}

void UpgradeInput::AssginCommandToTriggerPadY()
{
	IUpgradeCommand* command = new UpgradeSelectCommand();
	triggerPadY_ = command;
}
