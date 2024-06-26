#include "ClearScene.h"

#include "Game/Scenes/Title/TitleScene.h"
#include "Game/Scenes/Game/GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void ClearScene::Initialize()
{
	// 画面全体
	backSprite_.texture = Resource::LoadTexture("GameClear.png");
	backSprite_.isUI = true;
	backSprite_.isActive = true;
	// 適当にサイズを画面全体に拡げる
	Vector2 spSize = backSprite_.texture.t.GetSize();
	backSprite_.transform.scale.x = 1.0f / spSize.x * 1980.0f;
	backSprite_.transform.scale.y = 1.0f / spSize.y * 1080.0f;

	// ボタン選択
	toTitleSprite_.texture = Resource::LoadTexture("Text/BackForTitle.png");
	toTitleSprite_.anchorPoint = { 0.5f,0.5f };
	toTitleSprite_.isUI = true;
	toTitleSprite_.isActive = true;
	toTitleSprite_.transform.translation.x = 1980.0f / 2.0f;
	toTitleSprite_.transform.translation.y = 1080.0f / 2.0f - spriteWidth + spriteOffset;
	toTitleSprite_.commonColor = new Color(0xAAAAAAFF);

	// ボタン選択
	toGameSprite_.texture = Resource::LoadTexture("Text/Retry.png");
	toGameSprite_.anchorPoint = { 0.5f,0.5f };
	toGameSprite_.isUI = true;
	toGameSprite_.isActive = true;
	toGameSprite_.transform.translation.x = 1980.0f / 2.0f;
	toGameSprite_.transform.translation.y = 1080.0f / 2.0f + spriteWidth + spriteOffset;
	toGameSprite_.commonColor = new Color(0xAAAAAAFF);

	// ボタン選択
	cursolSprite_.texture = Resource::LoadTexture("cursor.png");
	cursolSprite_.anchorPoint = { 0.5f,0.5f };
	cursolSprite_.isUI = true;
	cursolSprite_.isActive = true;
	cursolSprite_.transform.translation.x = 1980.0f / 2.0f - 450;
	cursolSprite_.transform.translation.y = 1080.0f / 2.0f - spriteWidth + spriteOffset;
	cursolSprite_.transform.scale = { 0.5f,0.5f };
	cursolSprite_.commonColor = new Color(0xAAAAAAFF);

	sceneTransition_ = std::make_unique<SceneTransition>();
	sceneTransition_->Initialize();
}

// 更新
void ClearScene::Update()
{
#ifdef DEMO

	ImGui::Begin("Scene");
	ImGui::Text("Clear");
	ImGui::Text("Space : NextScene");
	ImGui::Text("LEFT or RIGHT : Title or Game");
	ImGui::Text("Next : %s", choise_ == 0 ? "Title" : "Game");
	ImGui::End();

#endif // DEMO

	// 選択肢を与える
	// 左
	if (Keyboard::GetTrigger(DIK_W) || Keyboard::GetTrigger(DIK_UP) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_UP))
	{
		choise_ = 0;
		cursolSprite_.transform.translation.y = 1080.0f / 2.0f - spriteWidth + spriteOffset;
	}
	//　右
	else if (Keyboard::GetTrigger(DIK_S) || Keyboard::GetTrigger(DIK_DOWN) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		choise_ = 1;
		cursolSprite_.transform.translation.y = 1080.0f / 2.0f + spriteWidth + spriteOffset;
	}


	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		sceneTransition_->Start();
	}
	sceneTransition_->Update();

	if (sceneTransition_->GetIsSceneChange()) {
		if (choise_ == 0)
			nextSceneFunction = []() {return new TitleScene; };
		else
			nextSceneFunction = []() {return new GameScene; };
	}
}