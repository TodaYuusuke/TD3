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
	//BGM
	BGM = std::make_unique<LWP::Resource::Audio>();
	BGM->Load("BGM/Pop_Step.mp3");
	BGMvolume = 0.0f;

	BGMt = 0.0f;
	IsSceneChangeBegin = false;
	IsSceneChangeEnd = true;

	chooseSE = std::make_unique<LWP::Resource::Audio>();
	chooseSE->Load("menu/patternA.mp3");
}

// 更新
void TitleScene::Update()
{
	if (IsloadEnd) {
		BGM->Play(BGMvolume, 255);
		IsloadEnd = false;
	}

	//だんだん音が上がる
	if (BGMt != 1.0f && IsSceneChangeEnd == true) {
		BGMt = (std::min)(BGMt + 0.01f, 1.0f);
		BGMvolume = Lerp(BGMvolume, 1.0f, BGMt);
	}
	else {
	IsSceneChangeEnd = false;
	BGMt = 0.0f;
	}

	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		sceneTransition_->Start();
		if (IsSceneChangeBegin == false) {
			chooseSE->Play();
		}
		IsSceneChangeBegin = true;
	}

	if (IsSceneChangeBegin == true) {
		//だんだん音が下がる
		BGMt = (std::min)(BGMt + 0.05f, 1.0f);
		BGMvolume = Lerp(BGMvolume, 0.0f, BGMt);
	}

	BGM->SetVolume(BGMvolume);

	sceneTransition_->Update();

	if (sceneTransition_->GetIsSceneChange()) {
		BGM->Stop();
		chooseSE->Stop();

		nextSceneFunction = []() {return new GameScene; };
	}
}