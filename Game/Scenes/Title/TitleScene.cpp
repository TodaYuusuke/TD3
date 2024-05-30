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
	// サイズ確保
	backSprite_.resize(NumSprite);

	// 画面全体
	backSprite_[BackGround].texture = Resource::LoadTexture("backGround.png");
	backSprite_[Text_TitleName].texture = Resource::LoadTexture("Text/titleName.png");
	backSprite_[Text_GamePad_A].texture = Resource::LoadTexture("Text/gamePad_A.png");
	for (int i = 0; i < NumSprite; i++) {
		backSprite_[i].isUI = true;
		backSprite_[i].isActive = true;
	}
	backSprite_[Text_GamePad_A].anchorPoint = { 0.5f,0.5f };
	backSprite_[Text_GamePad_A].transform.scale = { 0.65f, 0.65f, 0.65f };
	backSprite_[Text_GamePad_A].transform.translation= { 1980.0f / 2.0f, 1020.0f / 2.0f + 150.0f, -1000.0f };

	sceneTransition_ = std::make_unique<SceneTransition>();
	sceneTransition_->Initialize();
}

// 更新
void TitleScene::Update()
{
	animFrame_ += lwp::GetDefaultDeltaTimeF() * 60;
	// タイトル名を上下に動かす
	backSprite_[Text_TitleName].transform.translation.y += sinf(animFrame_ * M_PI / 60) * 0.1f;

	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		sceneTransition_->Start();
	}

	sceneTransition_->Update();

	if (sceneTransition_->GetIsSceneChange()) {
		nextSceneFunction = []() {return new GameScene; };
	}
}