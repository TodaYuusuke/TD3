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
	backSprite_.texture = Resource::LoadTexture("Title.png");
	backSprite_.isUI = true;
	backSprite_.isActive = true;
	// 適当にサイズを画面全体に拡げる
	Vector2 spSize = backSprite_.texture.t.GetSize();
	backSprite_.transform.scale.x = 1.0f / spSize.x * 1980.0f;
	backSprite_.transform.scale.y = 1.0f / spSize.y * 1080.0f;

	sceneTransition_ = std::make_unique<SceneTransition>();
	sceneTransition_->Initialize();

	audio = std::make_unique<LWP::Resource::Audio>();
	audio->Load("fanfare.wav");
}

// 更新
void TitleScene::Update()
{
	/*audio = std::make_unique<LWP::Resource::Audio>();
	audio->Load("fanfare.wav");*/
	//TODO : 一度だけ再生しないと重なる

	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		sceneTransition_->Start();
		if (aaa == 0) {
			audio->Play(0.2f);
			aaa = 1;
		}
	}

	sceneTransition_->Update();

	if (sceneTransition_->GetIsSceneChange()) {
		nextSceneFunction = []() {return new GameScene; };
		audio->Stop();
	}
}