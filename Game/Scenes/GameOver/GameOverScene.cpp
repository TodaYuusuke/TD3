#include "GameOverScene.h"

#include "Game/Scenes/Title/TitleScene.h"
#include "Game/Scenes/Game/GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void GameOverScene::Initialize()
{

}

// 更新
void GameOverScene::Update()
{
	ImGui::Begin("Scene");
	ImGui::Text("GameOver");
	ImGui::Text("Space : NextScene");
	ImGui::Text("LEFT or RIGHT : Title or Game");
	ImGui::Text("Next : %s", choise_ == 0 ? "Title" : "Game");
	ImGui::End();

	// 選択肢を与える
	// 左
	if (Keyboard::GetTrigger(DIK_A) || Keyboard::GetTrigger(DIK_LEFT) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		choise_ = 0;
	}
	//　右
	else if (Keyboard::GetTrigger(DIK_D) || Keyboard::GetTrigger(DIK_RIGHT) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		choise_ = 1;
	}

	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		if (choise_ == 0)
			nextSceneFunction = []() {return new TitleScene; };
		else
			nextSceneFunction = []() {return new GameScene; };
	}
}