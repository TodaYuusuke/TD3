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
	//scUpgrade_ = std::make_unique<UpgradeScreen>();
	//scUpgrade_->Initialize();

	//level_->SetUpgradeScreen(scUpgrade_.get());
	upgradeManager_ = std::make_unique<L::UpgradeManager>();
	upgradeManager_->Init();

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
	ImGui::Begin("Scene");
	ImGui::Text("Game");
	ImGui::End();

	// 時間を計測
	// チュートリアルの時は計測しない
	if (!enemyManager_->GetIsTutorial()) {
		gameTimer_->Update();
	}

	// タイマーのカウントが終了したとき
	if (gameTimer_->isEnd_)
	{
		ImGui::Begin("Scene");
		ImGui::Text("Space");
		ImGui::End();

		// プレイヤーが生きているとき
		if (player_->flag_.isAlive_)
		{
			// クリアしたときの処理

			ImGui::Begin("Scene");
			ImGui::Text("Clear");
			ImGui::End();

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

			ImGui::Begin("Scene");
			ImGui::Text("GameOver");
			ImGui::End();

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
