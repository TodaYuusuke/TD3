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
	backSprite_[0].texture = Resource::LoadTexture("backGround.png");
	backSprite_[1].texture = Resource::LoadTexture("Text/GameClear.png");
	for (int i = 0; i < 2; i++)
	{
		backSprite_[i].isUI = true;
		backSprite_[i].isActive = true;
	}
	// 適当にサイズを画面全体に拡げる
	//Vector2 spSize = backSprite_.texture.t.GetSize();
	//backSprite_.transform.scale.x = 1.0f / spSize.x * 1980.0f;
	//backSprite_.transform.scale.y = 1.0f / spSize.y * 1080.0f;

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
	cursolSprite_.transform.rotation.z = -(float)std::numbers::pi / 2.0f;
	cursolSprite_.transform.scale = { 0.5f,0.5f };
	cursolSprite_.commonColor = new Color(0xAAAAAAFF);

	sceneTransition_ = std::make_unique<SceneTransition>();
	sceneTransition_->Initialize();
	//BGM
	BGM = std::make_unique<LWP::Resource::Audio>();
	BGM->Load("fanfare.wav");
	BGMvolume = 0.2f;
	BGM->Play(BGMvolume, 255);
	BGMt = 0.0f;
	IsSceneChangeBegin = false;
	IsSceneChangeEnd = true;

	serectSE = std::make_unique<LWP::Resource::Audio>();
	serectSE->Load("menu/cursor.mp3");
	chooseSE = std::make_unique<LWP::Resource::Audio>();
	chooseSE->Load("menu/patternA.mp3");
}

// 更新
void ClearScene::Update()
{
	animFrame_ += lwp::GetDefaultDeltaTimeF() * 60;
	// タイトル名を上下に動かす
	backSprite_[1].transform.translation.y += sinf(animFrame_ * M_PI / 60) * 0.1f;

#ifdef DEMO

	ImGui::Begin("Scene");
	ImGui::Text("Clear");
	ImGui::Text("Space : NextScene");
	ImGui::Text("LEFT or RIGHT : Title or Game");
	ImGui::Text("Next : %s", choise_ == 0 ? "Title" : "Game");
	ImGui::End();

#endif // DEMO

	//だんだん音が上がる
	if (BGMt != 1.0f && IsSceneChangeEnd == true)
	{
		BGMt = (std::min)(BGMt + 0.01f, 1.0f);
		BGMvolume = Lerp(BGMvolume, 1.0f, BGMt);
	}
	else
	{
		IsSceneChangeEnd = false;
	}

	// 選択肢を与える
	// 上
	if (Keyboard::GetTrigger(DIK_W) || Keyboard::GetTrigger(DIK_UP) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_UP) || 0.0f < Pad::GetLStick().y){

		if(choise_ == 1){
			serectSE->Play();
		}
		choise_ = 0;
		cursolSprite_.transform.translation.y = 1080.0f / 2.0f - spriteWidth + spriteOffset;


	}
	//　下
	else if (Keyboard::GetTrigger(DIK_S) || Keyboard::GetTrigger(DIK_DOWN) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN) || Pad::GetLStick().y < 0.0f){
		if (choise_ == 0) {
			serectSE->Play();
		}
		choise_ = 1;
		cursolSprite_.transform.translation.y = 1080.0f / 2.0f + spriteWidth + spriteOffset;
	}


	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		if (IsSceneChangeBegin == false)
		{
			chooseSE->Play();
		}
		sceneTransition_->Start();
		IsSceneChangeBegin = true;
	}
	if (IsSceneChangeBegin == true)
	{
		//だんだん音が下がる
		BGMt = (std::min)(BGMt + 0.05f, 1.0f);
		BGMvolume = Lerp(BGMvolume, 0.0f, BGMt);
	}

	BGM->SetVolume(BGMvolume);

	sceneTransition_->Update();

	if (sceneTransition_->GetIsSceneChange())
	{
		if (choise_ == 0)
		{
			BGM->Stop();
			chooseSE->Stop();
			nextSceneFunction = []() {return new TitleScene; };
		}
		else
		{
			BGM->Stop();
			chooseSE->Stop();
			nextSceneFunction = []() {return new GameScene; };
		}
	}
}