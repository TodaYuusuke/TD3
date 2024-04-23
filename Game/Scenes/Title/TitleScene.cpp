#include "TitleScene.h"

#include "Game/Scenes/Game/GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void TitleScene::Initialize()
{
	// 画面全体
	backSprite_.texture = Resource::LoadTexture("white.png");
	backSprite_.isUI = true;
	backSprite_.isActive = true;
	// 適当にサイズを画面全体に拡げる
	Vector2 spSize = backSprite_.texture.t->GetTextureSize();
	backSprite_.transform.scale.x = 1.0f / spSize.x * 1980.0f;
	backSprite_.transform.scale.y = 1.0f / spSize.y * 1080.0f;

	// ボタン選択
	buttonSprite_.texture = Resource::LoadTexture("white.png");
	buttonSprite_.anchorPoint = { 0.5f,0.5f };
	buttonSprite_.isUI = true;
	buttonSprite_.isActive = true;
	buttonSprite_.transform.translation.x = 1980.0f * 0.5f;
	buttonSprite_.transform.translation.y = 1080.0f * 0.5f;
	buttonSprite_.commonColor = new Color(0x111111FF);
}

// 更新
void TitleScene::Update()
{
	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		nextSceneFunction = []() {return new GameScene; };
	}
}