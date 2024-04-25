#include "GameScene.h"

#include "Game/Scenes/Clear/ClearScene.h"
#include "Game/Scenes/GameOver/GameOverScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void GameScene::Initialize()
{
	// タイマー
	gameTimer_ = GameTimer::GetInstance();
	gameTimer_->Initialize();

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetScene(this);

	// 地面
	ground.LoadFile("ground/ground.obj");
	ground.transform.translation.y = -0.5f;
	ground.transform.scale = { 10.0f,0.1f, 10.0f };
	ground.name = "Ground";
	ground.material.enableLighting = true;
	ground.material.uvTransform.scale = { 20,20,0 };
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetCameraAddress(&mainCamera);
	followCamera_->SetTarget(player_->GetWorldTransform());
	followCamera_->SetPlayer(player_.get());
	// カメラを少し上に上げる
	followCamera_->pCamera_->transform.rotation.x = 0.3f;
	player_->SetCameraPointer(followCamera_.get());

	// 経験値タンク
	expManager_ = std::make_unique<ExpManager>();
	expManager_->Initialize();

	// エネミーマネージャ
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->SetCamera(followCamera_.get());
	// 経験値マネージャーをエネミーマネージャーに設定
	enemyManager_->SetExpManager(expManager_.get());
	enemyManager_->Init();


	// アップグレード
	upgradeManager_ = std::make_unique<L::UpgradeManager>();
	upgradeManager_->Init();

	tutorialSprite1.texture = Resource::LoadTexture("Text/tutorial1.png");
	tutorialSprite1.isUI = true;
	tutorialSprite1.isActive = true;
	tutorialSprite1.transform.translation.y = 120.0f;
	tutorialSprite2.texture = Resource::LoadTexture("Text/tutorial2.png");
	tutorialSprite2.isUI = true;
	tutorialSprite2.isActive = true;
	tutorialSprite2.transform.translation.y = 200.0f;

	backSprite_.texture = Resource::LoadTexture("white.png");
	backSprite_.isUI = true;
	backSprite_.isActive = false;
	backSprite_.anchorPoint = { 0.5f,0.5f };
	backSprite_.transform.scale.x = 1.2f;
	backSprite_.transform.scale.y = 0.5f;
	backSprite_.transform.translation = { 1600.0f,100.f,0.0f };
	backSprite_.commonColor = new Color(0xAAAAAAFF);

	// いったん五分
	gameTimer_->Reset(180);
	// ゲームが始まってから計測開始
	gameTimer_->Start();

	// 点光源
	sun_.transform.translation.y = 30.0f;
	sun_.intensity = 2.0f;
	sun_.radius = 100.0f;
	sun_.decay = 1.5f;

}

// 更新
void GameScene::Update()
{
#ifdef DEMO
	ImGui::Begin("Scene");
	ImGui::Text("Game");
	ImGui::End();
#endif

	// 時間を計測
	// チュートリアルの時は計測しない
	if (!enemyManager_->GetIsTutorial()) {
		gameTimer_->Update();
		tutorialSprite1.isActive = false;
		tutorialSprite2.isActive = false;
		backSprite_.isActive = true;
	}

	// タイマーのカウントが終了したとき
	if (gameTimer_->isEnd_)
	{
#ifdef DEMO
		ImGui::Begin("Scene");
		ImGui::Text("Space");
		ImGui::End();
#endif
		

		// プレイヤーが生きているとき
		if (player_->flag_.isAlive_)
		{
			// クリアしたときの処理
#ifdef DEMO
			ImGui::Begin("Scene");
			ImGui::Text("Clear");
			ImGui::End();
#endif


			// 何か演出を出す
			if (player_->ClearAnime())
			{
				// タイマーを消す
				gameTimer_->isActive_ = false;
				// 描画を消す
				gameTimer_->Update();
				nextSceneFunction = []() {return new ClearScene; };
			}
			return;
		}
		// プレイヤーが死んでいた時
		else
		{
			// ゲームオーバーしたときの処理
#ifdef DEMO
			ImGui::Begin("Scene");
			ImGui::Text("GameOver");
			ImGui::End();
#endif


			// 何か演出を出す
			if (Keyboard::GetTrigger(DIK_SPACE) ||
				Pad::GetTrigger(XINPUT_GAMEPAD_A))
			{
				// タイマーを消す
				gameTimer_->isActive_ = false;
				// 描画を消す
				gameTimer_->Update();
				nextSceneFunction = []() {return new GameOverScene; };
			}
			return;
		}
	}

	// スローを確認
	if (player_->GetIsJustSlashing())
	{
		time_ += lwp::GetDeltaTimeF();
		if (cTIMESLOW_ <= time_)
		{
			player_->EndJust();
			EndJustSlash();
		}
	}

	// デバッグ
//#ifdef DEMO
	upgradeManager_->DebugWindow(player_.get());
//#endif // DEMO


	if (!upgradeManager_->GetLevelUpFlag())
	{
		// プレイヤー
		player_->Update();

		// 追従カメラ
		followCamera_->Update();

		//mainCamera->transform = followCamera_->camera_.transform;

		enemyManager_->Update();

		// 敵が死んだときに出てくるので敵の更新の後
		// 経験値を更新
		expManager_->Update();

	}
	else
	{
		upgradeManager_->Update(player_.get());
	}
}

void GameScene::StartJustSlash()
{
	//time_ = 0.0f;
	//LWP::Info::SetDeltaTimeMultiply(0.1f);
	followCamera_->StartSlash();
}

void GameScene::EndJustSlash()
{
	LWP::Info::SetDeltaTimeMultiply(1.0f);
	followCamera_->EndSlash();
}
